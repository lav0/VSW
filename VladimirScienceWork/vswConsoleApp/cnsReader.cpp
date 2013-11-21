#include "stdafx.h"

#include "cnsReader.h"

//=============================================================================
cnsReader::cnsReader(string a_file_name)
//
// lav 19/11/13 written.
//
{
  fstream m_file_name(a_file_name);
  m_b_is_valid = m_file_name.is_open(); 

  if (!m_b_is_valid) {
    return;
  }
  stringstream buffer;
  buffer << m_file_name.rdbuf();

  boost::split(m_nodes, buffer.str(), boost::is_any_of("$"));

  m_b_is_valid &= !m_nodes.empty();

  trim_list();

  m_iterator = m_nodes.begin();
}

//=============================================================================
void cnsReader::trim_list()
//
// lav 19/11/13 written.
//
{
  vector<string>::iterator itr = m_nodes.begin();
  while (itr != m_nodes.end()) {
    string str_clear;
    for (size_t i=0; i<itr->size(); i++) {
      char c_current = itr->at(i);
      if (c_current == ',') {
        str_clear += '.';
      } else if (c_current != ' ' && c_current != '\n' && c_current != '\r') {
        str_clear += c_current;
      }
    }
    itr->operator=(str_clear);
    ++itr;
  }
}

//=============================================================================
bool cnsReader::get_next_point(
  boost::shared_ptr<matPoint2D>& a_pn_out
)
//
// lav 21/11/13 written.
//
{
  vector<string> list_by_hash;
  boost::split(list_by_hash, get_next_node(), boost::is_any_of("#"));
  
  if (list_by_hash.size() != 2) {
    return false;
  }

  try {

    boost::algorithm::trim(list_by_hash[0]);
    boost::algorithm::trim(list_by_hash[1]);
    a_pn_out->X = boost::lexical_cast<double>(list_by_hash[0]);
    a_pn_out->Y = boost::lexical_cast<double>(list_by_hash[1]);

  } catch(...) {
    return false;
  }

  return true;
}

//=============================================================================
string cnsReader::get_next_node()
//
// lav 19/11/13 written.
//
{
  if (!m_b_is_valid) {
    return "";
  }
  string str_res = *m_iterator;

  m_b_is_valid = ++m_iterator != m_nodes.end();

  return str_res;
}

//=============================================================================