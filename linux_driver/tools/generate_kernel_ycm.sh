#! /bin/bash
set -e 
eval $(cat Makefile | grep "^KERNEL_SRC=" | sed 's/\/$//g')

INCLUDE_FILES=$(cat .clang_complete | \
    grep '^-[iI]' | \
    sed "s/^-I\([^/]\)/-I$(echo $KERNEL_SRC | \
    sed 's/\//\\\//g')\/\1/g")

# Generate .ycm_extra_conf.py file
[[ -e .ycm_extra_conf.pyc ]] && rm .ycm_extra_conf.pyc
sed "s/__INCLUDE_PATH__/$(echo $INCLUDE_FILES | \
    sed 's/\//\\\//g' | sed 's/^\(.*\)$/'\''\1'\'',/g' | \
    sed 's/ -\([iI]\)/'\'',\\\n'\''-\1/g')/g" < ~/workspace/scripts/ycm_extra_conf.py.template.kernel > .ycm_extra_conf.py

# Generate cscope database
gcc ${INCLUDE_FILES} -M $(find -name "*.c") | \
    sed -e 's/[\\ ]/\n/g' | \
    sed -e '/^$/d' -e '/\.o:[ \t]*$/d' > cscope.files

cscope -b -q -k

# Generate ctags database
cat cscope.files | ctags -L - --c++-kinds=+pl --fields=+ilaS --extra=+q
