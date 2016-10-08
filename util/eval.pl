#!/usr/bin/perl

$ref = $ARGV[0];
$hyp = $ARGV[1];

open REF, $ref or die $!;
open HYP, $hyp or die $!;

while(<REF>){
    chop;
    ($word_ref, $pos_ref) = split;
    
    $hyp = <HYP>;
    chop $hyp;
    ($word_hyp, $pos_hyp) = split / /, $hyp;
#    print "word hyp = $word_hyp pos hyp = $pos_hyp\n";
    
    print "$word_ref $word_hyp\n";

    if($word_ref != $word_hyp){
	die "mismatch\n";
    }

    if(($word_ref ne "") && ($word_hyp ne "")){
	$words++;
	if($pos_ref != $pos_hyp){
#	    print "$pos_ref $pos_hyp\n";
	    $errors++;
	}
    }
    else{
	#print "fin de phrase\n";
}
}
$error_rate = $errors / $words;
$accuracy = 1 - $error_rate;
printf "errors = $errors words = $words accuracy = $accuracy\n";
