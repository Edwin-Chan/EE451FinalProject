#!/bin/bash
make all
rm -rf output
mkdir output
rm -rf log
mkdir log
rm -rf decode
mkdir decode
FILES="data/*"
OUTPUT_DIR="output/"
SOUT="_serial.out"
SLOG="_serial.log"
STALOG="_std.log"
PLOG="_para.log"
STAOUT="_std.out"
POUT="_para.out"
LOG_DIR="log/"
DECODE_DIR="decode/"
SDEC="_serial_decode.txt"
STADEC="_std_decode.txt"
PDEC="_para_decode.txt"

# EXECS="lzw_serial_encode
# lzw_standard_encode
# lzw_parallel_encode"
for f in $FILES 
do
    echo "Processing file $f..."
    FBASE=$(basename "$f" .txt)
    echo "Processing file base $FBASE"
    ./lzw_serial_encode $f $OUTPUT_DIR$FBASE$SOUT > $LOG_DIR$FBASE$SLOG
    ./lzw_serial_decode $OUTPUT_DIR$FBASE$SOUT $DECODE_DIR$FBASE$SDEC
    ./lzw_standard_encode 4 $f $OUTPUT_DIR$FBASE$STAOUT > $LOG_DIR$FBASE$STALOG
    ./lzw_standard_decode $OUTPUT_DIR$FBASE$STAOUT $DECODE_DIR$FBASE$STADEC
    ./lzw_parallel_encode 7 $f $OUTPUT_DIR$FBASE$POUT > $LOG_DIR$FBASE$PLOG
    ./lzw_parallel_decode $OUTPUT_DIR$FBASE$POUT $DECODE_DIR$FBASE$PDEC
done


