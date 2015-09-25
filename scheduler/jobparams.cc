/*---------------------------------------------------------------------------
* scheduler: Classes for handling a job.
* Copyright (C) 2015-2015 by Amal Medhi <amedhi@iisertvm.ac.in>.
* All rights reserved.
* Date:   2015-08-17 13:33:19
* Last Modified by:   amedhi
* Last Modified time: 2015-09-16 00:47:17
*----------------------------------------------------------------------------*/
// File: jobparms.cc 
// Implentation for JobParams Class 
#include <fstream>
#include <cctype>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include "jobparams.h"

namespace scheduler {

JobParams::JobParams(const std::string& inputfile): n_params(0), n_tasks(0)
{
  try {
    n_tasks = parse(inputfile);
    n_params = param_list.size();
    valid = true;
  }
  catch (JobParams::bad_input& input_error) {
    std::cout << input_error.message() << std::endl;
    valid = false;
  }
}

unsigned int JobParams::fill(const std::string& inputfile)
{
  try {
    n_tasks = parse(inputfile);
    n_params = param_list.size();
    valid = true;
  }
  catch (JobParams::bad_input& input_error) {
    std::cout << input_error.message() << std::endl;
    valid = false;
    n_tasks = n_params = 0;
  }

  for (unsigned n=0; n<n_tasks; ++n) get_task_param(n);
  
  return n_tasks;
}

unsigned int JobParams::parse(const std::string& inputfile)
{
  std::ifstream fin;
  infile = inputfile;
  if (infile.length() == 0) {
    std::cout << " input file not specified\n"; 
    std::cout << " looking for default input file 'input.parm'..."; 
    infile = "input.parm";
  }
  else {
    std::cout << " looking for input file '" << infile << "'...";
  }

  fin.open(infile);
  if (fin.is_open()) {
    std::cout << " found\n reading input file '" << infile << "'...";
  }
  else {
    std::cout << " not found\n";
    throw bad_input("input file '"+infile+"' does not exist");
  }
  std::cout << "\n";

  // parsing starts
  unsigned line_no = 0;
  int n, nval;
  double numval, startv, stepv, endv, lim;
  const double eps = 1.0E-10;
  std::string::size_type pos;
  std::string line, pname, pval, pval_copy;
  std::string err = " **error: ";
  boost::char_separator<char> colon(";");
  boost::char_separator<char> comma(",");
  boost::tokenizer<boost::char_separator<char> >::iterator it;
  std::vector<bool> boo_vec;
  std::vector<double> num_vec;
  std::vector<std::string> str_vec;
  std::set<std::string> param_set;
  using key_boov_pair = std::pair<const std::string, std::vector<bool> >;
  using key_numv_pair = std::pair<const std::string, std::vector<double> >;
  using key_strv_pair = std::pair<const std::string, std::vector<std::string> >;
  using key_ptype_pair = std::pair<const std::string, param_t>;

  while (std::getline(fin,line)) {
    line_no++;
    // skip comments & blank lines
    pos = line.find_first_of("#");
    if (pos != std::string::npos) line.erase(pos);
    if (line.find_first_not_of(" ") == std::string::npos) continue;
    // first thing: '=' sign is a must
    pos = line.find_first_of("=");
    if (pos == std::string::npos) {
      throw bad_input("invalid syntax", line_no);
    }
    // take the parameter name & discard the '=' sign
    pname = line.substr(0, pos);
    boost::trim(pname);

    // rest of the line 
    line.erase(0, pos+1);
    boost::trim(line);

    // ';' at the end optional except for 'value list' assignment
    if (line.find_first_of(';') == line.length()-1) {
      line.pop_back();
      boost::trim(line);
    }

    // ',' separated range value
    if (line.find(",") != std::string::npos) {
      boost::trim(line);
      n = std::count(line.begin(), line.end(), ',');
      if (n!=2 || line.front()==',' || line.back()==',')
        throw bad_input("invalid range assignment syntax", line_no);
      // tokenize
      boost::tokenizer<boost::char_separator<char> > tokens(line, comma);

      // start val
      it = tokens.begin();
      if (it != tokens.end()) startv = std::stod(*it);
      else throw bad_input("invalid range assignment syntax", line_no);
      // step val
      if (++it != tokens.end()) stepv = std::stod(*it);
      else throw bad_input("invalid range assignment syntax", line_no);
      // end val
      if (++it != tokens.end()) endv = std::stod(*it);
      else throw bad_input("invalid range assignment syntax", line_no);

      // checks
      if (stepv > 0.0) {
        if (endv < startv) throw bad_input("invalid range assignment syntax", line_no);
      } 
      else if (stepv < 0.0) {
        if (endv > startv) throw bad_input("invalid range assignment syntax", line_no);
      }
      else {
        throw bad_input("zero step size in range assignment", line_no);
      }

      // store the numerical values
      if (param_set.find(pname) == param_set.end()) {
        param_set.insert(pname);
        param_list.push_back({pname, num_t, 0});
        num_params.insert(key_numv_pair(pname, num_vec));
      } 

      // number of values
      nval = round((fabs(endv - startv))/fabs(stepv));

      // store the values
      if (stepv > 0.0) {
        lim = endv + eps;
        for (n=0; n<nval+1; ++n) {
          numval = startv + stepv * n;
          if (numval >= lim) break;
          num_params[pname].push_back(numval);
        }
      }
      else {
        lim = endv - eps;
        for (n=0; n<nval+1; ++n) {
          numval = startv + stepv * n;
          if (numval <= lim) break;
          num_params[pname].push_back(numval);
        }
      }
      //std::cout << startv << " " << stepv << " " << endv << std::endl;
      continue; // to next line
    } // ',' separated values

    // value list: separated by ';' 
    if (line.find(';') == std::string::npos) line.push_back(';');

    if (line.find(";") != std::string::npos) {
      if (line.back() != ';') 
        throw bad_input("for value list, ';' mandatory after every value", line_no);
      // tokenize
      boost::tokenizer<boost::char_separator<char> > tokens(line, colon);
      for (it = tokens.begin(); it != tokens.end(); ++it) {
        pval = *it;
        boost::trim(pval);
        // if string types
        if (pval.find_first_of("'\"") != std::string::npos) {
          if (pval.front() != pval.back()) 
            throw bad_input("syntax error in defining string value", line_no);
          // discared quotes
          pval.erase(pval.begin()); pval.pop_back();
          boost::trim(pval);
          // YES/NO value?
          pval_copy = pval;
          boost::to_upper(pval_copy);
          if (pval_copy.compare("YES") == 0) {
            if (param_set.find(pname) == param_set.end()) {
              param_set.insert(pname);
              param_list.push_back({pname, bool_t, 0});
              boo_params.insert(key_boov_pair(pname, boo_vec));
            }
            boo_params[pname].push_back(true);
            continue;
          }
          if (pval_copy.compare("NO") == 0) {
            if (param_set.find(pname) == param_set.end()) {
              param_set.insert(pname);
              param_list.push_back({pname, bool_t, 0});
              boo_params.insert(key_boov_pair(pname, boo_vec));
            }
            boo_params[pname].push_back(false);
            continue;
          }
          // string
          if (param_set.find(pname) == param_set.end()) {
            param_set.insert(pname);
            param_list.push_back({pname, str_t, 0});
            str_params.insert(key_strv_pair(pname, str_vec));
          }
          str_params[pname].push_back(pval);
          continue;
        } // string types

        // if true/false or T/F value
        pval_copy = pval;
        boost::to_upper(pval_copy);
        if (pval_copy.compare("T")==0 || pval_copy.compare("TRUE")==0) {
          if (param_set.find(pname) == param_set.end()) {
            param_set.insert(pname);
            param_list.push_back({pname, bool_t, 0});
            boo_params.insert(key_boov_pair(pname, boo_vec));
          }
          boo_params[pname].push_back(true);
          continue;
        }
        if (pval_copy.compare("F")==0 || pval_copy.compare("FALSE")==0) {
          if (param_set.find(pname) == param_set.end()) {
            param_set.insert(pname);
            param_list.push_back({pname, bool_t, 0});
            boo_params.insert(key_boov_pair(pname, boo_vec));
          }
          boo_params[pname].push_back(false);
          continue;
        }

        // numerical value
        for (const char& ch : pval) 
          if (isspace(ch)) throw bad_input("invalid parameter value", line_no);
        try {numval = std::stod(pval);}
        catch(std::invalid_argument) {throw bad_input("invalid parameter value", line_no);}
        if (param_set.find(pname) == param_set.end()) {
          param_set.insert(pname);
          param_list.push_back({pname, num_t, 0});
          num_params.insert(key_numv_pair(pname, num_vec));
        } 
        num_params[pname].push_back(numval);
      } // all tokens in this line 
      continue; // to next line
    } // ';' separated values
    //std::cout << pname << " =" << line << std::endl;
  } // while(getline(line))

  // 'size' of each parameter & number of parameter sets 
  unsigned n_sets = 1;
  for (unsigned p=0; p<param_list.size(); ++p) {
    pname = param_list[p].name;
    //std::cout << pname << " = ";
    switch (param_list[p].type) {
      case bool_t:
        param_list[p].size = boo_params[pname].size();
        break;
      case num_t:
        param_list[p].size = num_params[pname].size();
        break;
      case str_t:
        param_list[p].size = str_params[pname].size();
        break;
    }
    //std::cout << param_list[p].size << "\n";
    n_sets = n_sets * param_list[p].size;
  }

  return n_sets;
} // JobParms::parse()

void JobParams::get_task_param(const unsigned& task_id)
{
  std::vector<unsigned> idx(param_list.size());
  for (unsigned& i : idx) i = 0;

  unsigned p = 0;
  std::string pname;

  // advance the 'indices' to the correct set for the task
  for (unsigned t=0; t<task_id; ++t) {
    for (p=0; p<param_list.size(); ++p) {
      if (idx[p]+1 < param_list[p].size) {
        idx[p]++;
        break;
      }
      idx[p] = 0;
    }
  }

  std::cout << "task = " << task_id+1 << "\n";
  unsigned i;
  for (p=0; p<param_list.size(); ++p) {
    pname = param_list[p].name;
    std::cout << pname << " = ";
    i = idx[p];
    switch (param_list[p].type) {
      case bool_t:
        std::cout << boo_params[pname][i] << std::endl;
        break;
      case num_t:
        std::cout << num_params[pname][i] << std::endl;
        break;
      case str_t:
        std::cout << str_params[pname][i] << std::endl;
        break;
    }
  }
  std::cout << "\n";
}

JobParams::bad_input::bad_input(const std::string& msg, const int& ln)
  : std::runtime_error(msg), lnum(ln)
{
}

std::string JobParams::bad_input::message(void) const
{
  std::string msg = " **bad_input: ";
  if (lnum >= 0) msg += "line " + std::to_string(lnum) + ": ";
  msg += what();
  return msg;
}

/*bool JobParams::parse_error(const int& lno, const std::string& msg) 
{
  std::cout << " **parse error: line " << lno << ": " << msg << std::endl;
  return false;
}*/


} // end namespace

