#!/bin/bash

set -o errexit

if [ $# -ne 1 ]; then
	echo 'Usage: $0 <encoded archive name>'
	exit 1
fi

encoded_archive="$1"
# convert back from base64 format
base64 --decode $encoded_archive >out.tar
# unarchive
tar -C ./images -xvf out.tar
# 

rm out.tar
#rm $encoded_archive

