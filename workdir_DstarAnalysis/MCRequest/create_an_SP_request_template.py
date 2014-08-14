#!/usr/bin/env python

import sys

filename = sys.argv[1]

output  = "# Who is requesting this SP?\n"
output += "requestor = \"Jose Benitez\"\n\n"

output += "# What are the SP releases for which this will be generated?\n"
output += "# There are only certain allowed values, so let's look at each one.\n"
output += "# Note that multiple entries are allowed.\n"
output += "# SP8 -  Run 1-5. Events will be processed with R18, and then reprocessed such \n"
output += "#        that it is the equivalent of R22.\n"
output += "# SP9 -  Run 6. Events will be processed with R22.\n"
output += "# SP10 - Run 1-6. Events will be processed with R24.\n"
output += "# Y2S -  Run 7. Events will be processed with R24 for the Y(2S) running conditions.\n"
output += "# Y3S -  Run 7. Events will be processed with R24 for the Y(3S) running conditions.\n"
output += "# WARNING!!! WARNING!!!\n"
output += "# If you want both Y2S and Y3S modes, those should be submitted in two\n"
output += "# separate requests.\n"
output += "sp_release = [\"SP8\", \"SP9\" ]\n\n"

output += "# What modes and how many events per fb^{-1} are requested?\n"
output += "# The syntax is [ [mode1, events/ifb], [mode2, events/ifb], ...]\n"
output += "modes = [ [9997, 500], \\\n"
output += "[9998, 500], \\\n"
output += "[9999, 500] ]\n\n"

output += "# What AWG is overseeing this request? This is good information to have in the\n"
output += "# event that an abnormally large request is made. It allows the SP manager to verify\n"
output += "# that this request is really needed.\n"
output += "awg = \"Quarkonium\"\n\n"

output += "# Any comments about this request can go here. For instance, does the reqestor only\n"
output += "# need this data for Run 6? Are there special tags for the software release.?\n"
output += "comments = \"Priority modes for a really important measurement.\"\n\n"

output += "# List the emails of all the individuals who should be notified when this request\n"
output += "# is actually allocated to the computer resources.\n"
output += "emails = [ \"noether@slac.stanford.edu\", \"galois@slac.stanford.edu\", \"devnull@gmail.com\" ]\n"

output_file = open(filename+".py","w+")
output_file.write(output)
output_file.close()
