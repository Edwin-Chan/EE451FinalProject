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
# SLOG="_serial.log"
# STALOG="_std.log"
# PLOG="_para.log"
LOG=".log"
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
    # echo "Processing file base $FBASE"
    # ./lzw_serial_encode $f $OUTPUT_DIR$FBASE$SOUT > $LOG_DIR$FBASE$SLOG
    echo "Serial:" >> $LOG_DIR$FBASE$LOG
    ./lzw_serial_encode $f $OUTPUT_DIR$FBASE$SOUT >> $LOG_DIR$FBASE$LOG
    ./lzw_serial_decode $OUTPUT_DIR$FBASE$SOUT $DECODE_DIR$FBASE$SDEC >> $LOG_DIR$FBASE$LOG
    # ./lzw_standard_encode 4 $f $OUTPUT_DIR$FBASE$STAOUT > $LOG_DIR$FBASE$STALOG
    echo "\nStandard, 2 threads:" >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_encode 2 $f $OUTPUT_DIR$FBASE$STAOUT >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_decode $OUTPUT_DIR$FBASE$STAOUT $DECODE_DIR$FBASE$STADEC >> $LOG_DIR$FBASE$LOG
    echo "\nStandard, 4 threads:" >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_encode 4 $f $OUTPUT_DIR$FBASE$STAOUT >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_decode $OUTPUT_DIR$FBASE$STAOUT $DECODE_DIR$FBASE$STADEC >> $LOG_DIR$FBASE$LOG
    echo "\nStandard, 8 threads:" >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_encode 8 $f $OUTPUT_DIR$FBASE$STAOUT >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_decode $OUTPUT_DIR$FBASE$STAOUT $DECODE_DIR$FBASE$STADEC >> $LOG_DIR$FBASE$LOG
    echo "\nStandard, 16 threads:" >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_encode 16 $f $OUTPUT_DIR$FBASE$STAOUT >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_decode $OUTPUT_DIR$FBASE$STAOUT $DECODE_DIR$FBASE$STADEC >> $LOG_DIR$FBASE$LOG
    echo "\nStandard, 32 threads:" >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_encode 32 $f $OUTPUT_DIR$FBASE$STAOUT >> $LOG_DIR$FBASE$LOG
    ./lzw_standard_decode $OUTPUT_DIR$FBASE$STAOUT $DECODE_DIR$FBASE$STADEC >> $LOG_DIR$FBASE$LOG
    # ./lzw_parallel_encode 7 $f $OUTPUT_DIR$FBASE$POUT > $LOG_DIR$FBASE$PLOG
    echo "\nParallel, 3 blocks:" >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_encode 3 $f $OUTPUT_DIR$FBASE$POUT >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_decode $OUTPUT_DIR$FBASE$POUT $DECODE_DIR$FBASE$PDEC >> $LOG_DIR$FBASE$LOG
    echo "\nParallel, 7 blocks:" >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_encode 7 $f $OUTPUT_DIR$FBASE$POUT >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_decode $OUTPUT_DIR$FBASE$POUT $DECODE_DIR$FBASE$PDEC >> $LOG_DIR$FBASE$LOG
    echo "\nParallel, 15 blocks:" >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_encode 15 $f $OUTPUT_DIR$FBASE$POUT >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_decode $OUTPUT_DIR$FBASE$POUT $DECODE_DIR$FBASE$PDEC >> $LOG_DIR$FBASE$LOG
    echo "\nParallel, 31 blocks:" >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_encode 31 $f $OUTPUT_DIR$FBASE$POUT >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_decode $OUTPUT_DIR$FBASE$POUT $DECODE_DIR$FBASE$PDEC >> $LOG_DIR$FBASE$LOG
    echo "\nParallel, 63 blocks:" >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_encode 63 $f $OUTPUT_DIR$FBASE$POUT >> $LOG_DIR$FBASE$LOG
    ./lzw_parallel_decode $OUTPUT_DIR$FBASE$POUT $DECODE_DIR$FBASE$PDEC >> $LOG_DIR$FBASE$LOG
done


