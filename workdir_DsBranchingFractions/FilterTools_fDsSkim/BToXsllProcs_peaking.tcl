## File containing common procs used by BToXsllUser

## proc createblists { name decau dauLists }

# create lists of various decay modes ($decay) for Semi-inclusive b->sll

global BToXsllProcs_peaking

if ![info exists BToXsllProcs_peaking] {


    set BToXsllProcs_peaking 1


    proc peakingDaughterLister { name dauList } {

	set modName ${name}Lister
	
	clone BtaDaughterLister $modName
	
	talkto $modName {
	    
	    inList set $name
	    
	    foreach list $dauList {
		
		dauLists set $list
	    }
	    
	    #verbose set true
	}
	
	seq append Sll_peakingSeq $modName
    }

    proc peakingcreateshortblists { name decay dauLists } {

	global XsllBtaCandLists_peaking
	
	mod clone SmpMakerDefiner $name
	
	seq append Sll_peakingSeq $name
	
	catch { setProduction $name }
	
	talkto $name {
	    
	    decayMode   set $decay

	    foreach dauList $dauLists { daughterListNames set $dauList }
	    
	    preFitSelectors    set "Mes 5.1:"
	    
	    preFitSelectors    set "DeltaE -0.4:0.4"
	    
	    fittingAlgorithm   set "TreeFitter"
	    
	    fitConstraints     set "Geo"
	    
	    postFitSelectors   set "Mes 5.15:"

	    postFitSelectors   set "DeltaE -0.35:0.35"
	    
	    postFitSelectors   set "mHad    0.0:2.3"

	    postFitSelectors   set "ProbChiSq 0.000000000001:1"

	    #verbose            set true

	}

	lappend XsllBtaCandLists_peaking $name
	
    }

    proc peakingcreateshortbkslists { name decay dauLists } {

	global XsllBtaCandListsKs_peaking
	
	mod clone SmpMakerDefiner $name
	
	seq append Sll_peakingSeq $name
	
	catch { setProduction $name }
	
	talkto $name {
	    
	    decayMode   set $decay
	    
	    foreach dauList $dauLists { daughterListNames set $dauList }
	    
	    preFitSelectors    set "Mes 5.1:"
	    
	    preFitSelectors    set "DeltaE -0.4:0.4"

	    fittingAlgorithm   set "TreeFitter"

	    fitConstraints     set "Geo"
	    
	    postFitSelectors   set "Mes 5.15:"

	    postFitSelectors   set "DeltaE -0.35:0.35"

	    postFitSelectors   set "mHad    0:2.3"

	    postFitSelectors   set "ProbChiSq 0.000000000001:1"
	    
	    #verbose            set true

	}

	lappend XsllBtaCandListsKs_peaking $name

    }

    proc peakingcreatelongblists { name decay dauLists } {

	global XsllBtaCandLists_peaking

	mod clone SmpMakerDefiner $name
	
	seq append Sll_peakingSeq $name
	
	catch { setProduction $name }
	
	talkto $name {
	    
	    decayMode   set $decay
	    
	    foreach dauList $dauLists { daughterListNames set $dauList }
	    
	    preFitSelectors    set "Mes 5.1:"
	    
	    preFitSelectors    set "DeltaE -0.4:0.4"
	    
	    fittingAlgorithm   set "TreeFitter"
	    
	    fitConstraints     set "Geo"
	    
	    postFitSelectors   set "Mes 5.15:"
	    
	    postFitSelectors   set "DeltaE -0.35:0.35"
	    
	    postFitSelectors   set "mHad    0:2.3"
	    
	    postFitSelectors   set "ProbChiSq 0.000000000001:1"
	    
	    #verbose            set true
	    
	}
	
	lappend XsllBtaCandLists_peaking $name
	
    }

    proc peakingcreatelongbkslists { name decay dauLists } {
	
	global XsllBtaCandListsKs_peaking

	mod clone SmpMakerDefiner $name
	
	seq append Sll_peakingSeq $name
	
	catch { setProduction $name }
	
	talkto $name {
	    
	    decayMode   set $decay
	    
	    foreach dauList $dauLists { daughterListNames set $dauList }
	    
	    preFitSelectors    set "Mes 5.1:"
	    
	    preFitSelectors    set "DeltaE -0.4:0.4"
	    
	    fittingAlgorithm   set "TreeFitter"
	    
	    fitConstraints     set "Geo"
	    
	    postFitSelectors   set "Mes 5.15:"
	    
	    postFitSelectors   set "DeltaE -0.35:0.35"
	    
	    postFitSelectors   set "mHad    0:2.3"
	    
	    postFitSelectors   set "ProbChiSq 0.000000000001:1"
	    
	    #verbose            set true
	    
	}
	
	lappend XsllBtaCandListsKs $name
	
    }

    proc peakingcreatebglists { name decay dauLists } {
	
	global XsllBtaBGLists_peaking
	
	mod clone SmpMakerDefiner $name
	
	seq append Sll_peakingSeq $name
	
	catch { setProduction $name }
	
	talkto $name {
	    
	    decayMode   set $decay
	    
	    foreach dauList $dauLists { daughterListNames set $dauList }
	    
	    preFitSelectors    set "Mes 5.1:"
	    
	    preFitSelectors    set "DeltaE -0.35:0.35"
	    
	    fittingAlgorithm   set "Add4"
	    
	    postFitSelectors   set "mHad    0:2.3";
	    
	}
	
	lappend XsllBtaBGLists_peaking $name

    }

}


