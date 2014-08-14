# Who is requesting this SP?
requestor = "Jose Benitez"

# What are the SP releases for which this will be generated?
# There are only certain allowed values, so let's look at each one.
# Note that multiple entries are allowed.
# SP8 -  Run 1-5. Events will be processed with R18, and then reprocessed such 
#        that it is the equivalent of R22.
# SP9 -  Run 6. Events will be processed with R22.
# SP10 - Run 1-6. Events will be processed with R24.
# Y2S -  Run 7. Events will be processed with R24 for the Y(2S) running conditions.
# Y3S -  Run 7. Events will be processed with R24 for the Y(3S) running conditions.
# WARNING!!! WARNING!!!
# If you want both Y2S and Y3S modes, those should be submitted in two
# separate requests.
sp_release = ["SP8", "SP9" ]

# What modes and how many events per fb^{-1} are requested?
# The syntax is [ [mode1, events/ifb], [mode2, events/ifb], ...]
modes = [ [9632, 4000], \
[9633, 4000], \
[9634, 4000], \
[9635, 4000] ]

# What AWG is overseeing this request? This is good information to have in the
# event that an abnormally large request is made. It allows the SP manager to verify
# that this request is really needed.
awg = "charm"

# Any comments about this request can go here. For instance, does the reqestor only
# need this data for Run 6? Are there special tags for the software release.?
comments = "Please generate as soon as possible. The .dec files have been committed to the HEAD today (Feb 19 2009)"

# List the emails of all the individuals who should be notified when this request
# is actually allocated to the computer resources.
emails = [ "benitezj@slac.stanford.edu", "antimo@slac.stanford.edu","galois@slac.stanford.edu", "devnull@gmail.com" ]
