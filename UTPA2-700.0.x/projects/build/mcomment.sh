# How to use:
# arg1 = Match pattern
# arg2 = The target path to match
# arg3 = Additional coment

PATTERN=$1
target_source=$2
GREP_PRE_PATTERN='[ ]+'
GREP_POST_PATTERN='[ ]*\([ &#a-zA-Z_*(),0-9\[\]]*[;]'
M_GREP_POST_PATTERN='[ ]*\([ &#a-zA-Z_*(),0-9\[\]]*'
REG=")[ ]*;"
REPLACE_PAT=$3

function single_line_fun_match()
{ 
#  grep -vP '^//' $target_source -n
  grep -P "${GREP_PRE_PATTERN}"${PATTERN}"${GREP_POST_PATTERN}" $target_source -n  |
           awk -F: '{print $1}' > tmp

  while read line
  do
     sed -i  $line"s/^/\/\/$REPLACE_PAT/" $target_source
  done < tmp
}

function multi_line_fun_match()
{
   grep -P "${GREP_PRE_PATTERN}"${PATTERN}"${M_GREP_POST_PATTERN}" $target_source -n | 
   grep -vP "${GREP_PRE_PATTERN}"${PATTERN}"${M_GREP_POST_PATTERN}"";"               |
   grep -vP '#define'|
   grep -vP "${GREP_PRE_PATTERN}"${PATTERN}"${M_GREP_POST_PATTERN}""\)"|
   awk -F: '{print $1}' > tmp
   
  # echo "v02"
   while read line
   do
     sed -i  "${line},/)[ ]*;/s/^/\/\/$REPLACE_PAT/" $target_source 
   done < tmp
}
single_line_fun_match
multi_line_fun_match
