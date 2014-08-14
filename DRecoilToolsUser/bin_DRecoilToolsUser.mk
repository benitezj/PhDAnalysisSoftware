# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_DRecoilToolsUser.mk,v 1.3 2008/12/18 02:14:14 aidanrc Exp $
-include BTaggingSequences/main_BtsTaggingSequence.mk
override LINK_BTaggingSequences    += DRecoilToolsUserGNUmakefile
override LINK_BaBar                += DRecoilToolsUserGNUmakefile
override LINK_BetaMiniEnvSequences += DRecoilToolsUserGNUmakefile
override LINK_BetaMiniSequences    += DRecoilToolsUserGNUmakefile
override LINK_Framework            += DRecoilToolsUserGNUmakefile
override LINK_UsrTools             += DRecoilToolsUserGNUmakefile
override LINK_DRecoilToolsUser     += DRecoilToolsUserGNUmakefile
