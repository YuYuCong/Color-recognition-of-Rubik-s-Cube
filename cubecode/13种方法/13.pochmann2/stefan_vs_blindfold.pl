#--- Algorithms.
sub printAlg {
    $_ = x0F0DN0EB0H00N0B0R0KB0L0QB0G1A1M11C1I1E1OFI2DN2IEOB2H22N2B2GRM2MKGB2GLM2QBK23D3EN3E3B3N33H3;
    s!(\D*)(\d)!$1 . (ILOFCLKHRNQCL,OBIRALOBIRAL,CEJHPEIMIG,DFNRHRDKMQ)[$2] . reverse map {y/G-R/M-X/;$_} split "", " $1"!ge;
    print map { substr(UDFBLR,($_=ord()-65)%6,1) . substr "2 '",$_/6,1 } split "", (split)[$x]
}

#--- Orient.
for $x (1..18) {
    until( grep /^$ARGV[$x]$/, @goal=(UF,UR,UB,UL,DF,DR,DB,DL,FR,FL,BR,BL,UFR,URB,UBL,ULF,DRF,DFL,DLB,DBR) ){
	printAlg;
	$ARGV[$x] =~ s/(.)(.+)/$2$1/;
	$ARGV[$x<12 ? 0 : 19] =~ s/(.+)(.)/$2$1/
    }
}

#--- Permute.
for (0..40) {
    for $w (0..19) {
	if( $ARGV[$w] ne $goal[$w] ){
	    $x = 0;
	    until( $ARGV[$w] eq $goal[$x] ){ $x++ }
	    $x<12 ? (@ARGV[0,$x,12,15] = @ARGV[$x,0,15,12]) : (@ARGV[12,$x] = @ARGV[$x,12]);
	    printAlg $x+=18;
	    last
	}
    }
}
