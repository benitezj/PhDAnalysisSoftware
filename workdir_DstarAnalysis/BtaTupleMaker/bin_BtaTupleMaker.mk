# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_BtaTupleMaker.mk,v 1.5 2005/09/02 23:30:15 chcheng Exp $
-include BTaggingSequences/main_BtsTaggingSequence.mk
override LINK_BTaggingSequences    += BtaTupleMakerGNUmakefile
override LINK_BaBar                += BtaTupleMakerGNUmakefile
override LINK_BetaMiniEnvSequences += BtaTupleMakerGNUmakefile
override LINK_BetaMiniSequences    += BtaTupleMakerGNUmakefile
override LINK_Framework            += BtaTupleMakerGNUmakefile
override LINK_UsrTools             += BtaTupleMakerGNUmakefile
