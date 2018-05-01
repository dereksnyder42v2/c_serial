#!/bin/bash
# Usage: $0 <picture filename>
# Derek Snyder
# Mon 30 Apr 20:31:21 EDT 2018

set -o errexit # halt script if any command fails

if [ $# -ne 1 ]; then
	echo 'Usage: $0 <picture filename>'
	exit 1
fi

picfile_name="$1"

picfile="$picfile_name.jpeg"
picfile_compressed="$picfile.bz2"
picfile_cksum="$picfile.bz2.ck"
picfile_archive="$picfile_name.tar"

# take picture with picamera, name it $picFile
printf 'Taking picture...'
./picamera-takePicture.py $picfile 5
printf ' done.\n'

# compress picture
printf 'Compressing picture...'
bzip2 $picfile -c >$picfile_compressed
printf ' done.\n'

# compute checksum of compressed file
printf 'Computing checksum...'
cksum $picfile_compressed >$picfile_cksum
printf ' done.\n'

# create tarball of compressed picture & checksum
printf 'Creating archive...'
tar -cf $picfile_archive $picfile_compressed $picfile_cksum
printf ' done.\n'

# as if that weren't enough. have to convert archive w/ base64, so that
# terminal doesn't eat up some of our Bytes (i.e. chars with ctrl flow uses)
printf 'Performing base64 conversion of archive...'
base64 $picfile_archive >"$picfile_archive.b64"
printf ' done.\n'

printf 'Sending file now...'
# FINALLY, send the file down the wire
# TODO find better name for this binary. lol
#./Test "$picfile_archive.b64"
printf ' done.\n'

# Clean up after ourselves, but leave converted archive
printf 'Cleaning up files...'
rm $picfile
rm $picfile_compressed
rm $picfile_cksum
rm $picfile_archive
#rm "$picfile_archive.b64"
printf ' done.\n'

