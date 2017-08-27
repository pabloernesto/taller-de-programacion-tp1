#!/bin/bash
python ./cpplint.py --filter=`cat filter_options` \
    `find "../src" -regextype posix-egrep -regex '.*\.(h|hpp|c|cpp)'`
