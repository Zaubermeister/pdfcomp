## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

pdfcompdocdir = ${prefix}/doc/pdfcomp
pdfcompdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(pdfcompdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(pdfcompdocdir)
