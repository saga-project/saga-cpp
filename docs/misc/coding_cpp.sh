#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

Ok, I had a look at Gnu-indent, as it comes with RedHat 8.0.
This does much of the stuff we want.  I append a test input and
output file.  The program is clever, but of course not as clever
as a programmer.  You should re check your code by hand!

The options I used to resample our coding style are:

indent \
  -bl -bli0 -nce -cdw -cli2 -cbi0 -ss -npcs -cs -bs -saf -sai -saw \
	-di16 -nbc -npsl -bls -bad -bap -bbb -nsob -l80 -bbo -hnl -fca -fc1 \
	-lc70 -c33 -cd33 -cp33 -sc -i2 -ci8 -lp -ip2 -ts2 -ip8 -lps -pmt -nut \
	<infile>

Feedback to this configuration is welcome.
Useful other options which would be compatible to our CG are:

	swallow existing blank lines:            -sob
	put each function decl. arg on new line: -bfda 
	existing tab length:                     -ts2 or -ts8 ...

You can shield parts of your code:	
	disable: /* *INDENT-OFF* */
	         // *INDENT-OFF*
	enable:  /* *INDENT-ON* */
	         // *INDENT-ON*

					 
Indent does not understand C, so it does not know new types. From
the doc:

	# You must use the `-T' option to tell `indent' the name of all the
  # typenames in your program that are defined by `typedef'.  `-T' can be
  # specified more than once, and all names specified are used.  For
  # example, if your program contains
  # 
  #      typedef unsigned long CODE_ADDR;
  #      typedef enum {red, blue, green} COLOR;
  # 
	# you would use the options `-T CODE_ADDR -T COLOR'.

Also, indent does ONLY correct layout issues, and does NOT honor
the coding practices defined by our coding guides.  It  does not
comment your code, check for sensible bracketing of return,
checks for one-return-per-function and so on.  A pity ;-)

I did not extensively check c++ code.

However, this should make it pretty easy to convert existing code.  
Definitely better than any perl I could provide :-)

Cheers, Andre.




Quoting [Petr Holub] (May 09 2003):
> 
> > that conversion let me know.  I could provide some perl snippets
> > for that.
> 
> Another possibility is using GNU indent...
> 
> Petr
> 
> ================================================================
>                             Petr Holub
> CESNET z.s.p.o.                       Supercomputing Center Brno
> Zikova 4                             Institute of Compt. Science
> 162 00 Praha 6, CZ                            Masaryk University
> Czech Republic                     Botanicka 68a, 60200 Brno, CZ 
> e-mail: Petr.Holub@cesnet.cz               phone: +420-541512213
>                                              fax: +420-541212747
>                                        e-mail: hopet@ics.muni.cz
> 
> 
> _______________________________________________
> Ptb mailing list
> Ptb@gridlab.org
> https://www.gridlab.org/mailman/listinfo/ptb



-- 
+------------------------------------------------------------------+
| Andre Merzky                   | phon: ++49 - 30 - 84 1 85 - 339 |
| Zuse Institute Berlin (ZIB)    | fax : ++49 - 30 - 84 1 85 - 107 |
| - Scientific Visualization -   | mail: merzky@zib.de             |
| Takustr. 7 - D-14195 Berlin    | www : http://www.zib.de/merzky/ |
+------------------------------------------------------------------+
