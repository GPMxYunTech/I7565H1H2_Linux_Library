PJ = 'I7565H1H2_Linux_Library'
VERSION  = '0.4.5'
HTML_TITLE  = 'ICPDAS Libi7565 for Linux Source Code Tour v$(VERSION)'

all:
	#cd src && make
	cd examples && make

package:
	rm -f ../$(PJ)-$(VERSION)*
	make clean
	cd .. && ln -s $(PJ) $(PJ)-$(VERSION) && tar --exclude-from EXCLUDE -cf - $(PJ)-$(VERSION)/. | gzip -c > $(PJ)-$(VERSION).tar.gz
#	mv ../$(PJ)-$(VERSION).tar.gz .

#
clean:
	rm -f *.o *~ *.tmp \#* .\#*
	#cd src && make clean
	cd examples && make clean

#
html:
	make clean.html
	gtags
	htags -fnvat $(HTML_TITLE)
	lynx HTML/index.html

clean.html:
	rm -f -r GPATH GRTAGS GSYMS GTAGS HTML






