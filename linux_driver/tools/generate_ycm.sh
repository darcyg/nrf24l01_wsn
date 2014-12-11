#! /bin/sh

INCLUDE_FILES=$(cat .clang_complete | \
    grep '^-[iI]')

set -e

# Generate .ycm_extra_conf.py file
[[ -e .ycm_extra_conf.pyc ]] && rm .ycm_extra_conf.pyc
sed "s/__INCLUDE_PATH__/$(echo $INCLUDE_FILES | \
    sed 's/\//\\\//g' | sed 's/^\(.*\)$/'\''\1'\'',/g' | \
    sed 's/ -\([iI]\)/'\'',\\\n'\''-\1/g')/g" < ~/workspace/scripts/ycm_extra_conf.py.template > .ycm_extra_conf.py

INCLUDE_FILES_P=$(echo $INCLUDE_FILES | sed 's/ -I/'\'' -I'\''/g' | sed 's/^-I\(.*\)$/-I'\''\1'\''/g')
INCLUDE_FILES_P=$(echo $INCLUDE_FILES_P | sed 's/ \([^-]\)/\ \1/g')

# SOURCES=$(find -maxdepth 1 -name "*.c" -o -name "*.cpp")
# SOURCES=$(find -name "*.c" -o -name "*.cpp" -exec echo \"{}\" \;)
SOURCES=$(sed 's/^\(.*\)$/"\1"/g' .clang_source)

# Generate cscope database
if [ -n "$1" ]; then
    # find . -regex "^.*\.\(c\|h\|cpp\|hpp\)$" > tags.files
    sed 's/^\(.*\)$/"\1"/g' .clang_source > cscope.files
    # Generate ctags database
    cat .clang_source | ctags -L - --c++-kinds=+pl --fields=+iaS --extra=+q
else
    cmd="g++ $INCLUDE_FILES_P \
    -fPIC -std=c++11 -M $SOURCES | \
    sed 's/^ \(.*\)$/\1/g' | sed 's/ \\\\$//g' | \
    sed -e 's/\([^\\\\]\) /\1\n/g' | sed 's/\\\\ / /g' | \
    sed -e '/^$/d' -e '/\.o:[ \t]*$/d' > tags.files"

    eval $cmd
    sed 's/^\(.*\)$/"\1"/g' tags.files > cscope.files
    # Generate ctags database
    cat tags.files | ctags -L - --c++-kinds=+pl --fields=+iaS --extra=+q
fi;

cscope -b -q

