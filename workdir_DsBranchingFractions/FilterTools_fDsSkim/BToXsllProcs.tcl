## File containing common procs used by BToXsllUser

## proc createblists { name decau dauLists }

# create lists of various decay modes ($decay) for Semi-inclusive b->sll

global BToXsllProcs

if ![info exists BToXsllProcs] {
    

    set BToXsllProcs 1


    proc DaughterLister { name seq dauLists } {
	
	set modName ${name}Lister
	
	clone BtaDaughterLister $modName
	
	talkto $modName {
	    
	    inList set $name

	    foreach list $dauLists {

		dauLists set $list
	    }

	    #verbose set true
	}

	seq append $seq $modName
    }

    proc createklllists { name seq decay dauLists } {

	global XsllBtaCandLists

	mod clone SmpMakerDefiner $name
    
	seq append $seq $name
	
	catch { setProduction $name }
    
	talkto $name {
	
	    decayMode   set $decay
	
	    foreach dauList $dauLists { daughterListNames set $dauList }
	
 	    preFitSelectors    set "Mes 4.90:"
	    
 	    preFitSelectors    set "DeltaE -0.455:0.55"

	    fittingAlgorithm   set "TreeFitter"
	
 	    fitConstraints     set "Geo"
	
	    postFitSelectors   set "Mes 5.1:"
	
	    postFitSelectors   set "DeltaE -0.45:0.5"
	
	    postFitSelectors   set "mHad    0.0:2.3"
	
	    postFitSelectors   set "ProbChiSq 0.000000000001:"
	    
	    #verbose            set true

	}
    
	lappend XsllBtaCandLists $name

    }

    proc createkslllists { name seq decay dauLists } {

	global XsllBtaCandListsKs

	mod clone SmpMakerDefiner $name
    
	seq append $seq $name
	
	catch { setProduction $name }
    
	talkto $name {
	
	    decayMode   set $decay
	
	    foreach dauList $dauLists { daughterListNames set $dauList }
   
 	    preFitSelectors    set "Mes 4.90:"
	    
 	    preFitSelectors    set "DeltaE -0.455:0.55"
	
	    fittingAlgorithm   set "TreeFitter"
	
 	    fitConstraints     set "Geo"
	
	    postFitSelectors   set "Mes 5.1:"
	
	    postFitSelectors   set "DeltaE -0.45:0.5"
	
	    postFitSelectors   set "mHad    0:2.3"
	
	    postFitSelectors   set "ProbChiSq 0.000000000001:"

	    #verbose            set true

	}
    
	lappend XsllBtaCandListsKs $name

    }

    proc createsllbglists { name seq decay dauLists } {
	
 	global XsllBtaBGLists
	
 	mod clone SmpMakerDefiner $name
	
 	seq append $seq $name
	
 	catch { setProduction $name }
	
 	talkto $name {
	    
 	    decayMode   set $decay
	    
 	    foreach dauList $dauLists { daughterListNames set $dauList }
	
  	    preFitSelectors    set "Mes 5.05:"
	    
  	    preFitSelectors    set "DeltaE -0.455:0.55"
	    
 	    fittingAlgorithm   set "Add4"
	    
 	}
	
 	lappend XsllBtaBGLists $name
	
    }
    
}


