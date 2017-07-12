    NERsuite version 1.2.1


## About ##
 NERsuite is a named entity recognition toolkit.


## Copyright and license ##
 This toolkit includes and uses a modified version of the GENIA 3.0.1 tagger, and 
the GENIA tagger uses the information extracted from the WORDNET. The NERsuite also 
requires the installation of libLBFGS (http://www.chokkan.org/software/liblbfgs/) 
and CRFsuite (http://www.chokkan.org/software/crfsuite/). Dictionary compiler and 
tagger include CDBPP library (http://www.chokkan.org/software/cdbpp/). Therefore, 
the use of this toolkit need to follows the copyrights and the license terms of 
these softwares. For other parts of this toolkit, it follows the BSD license.


## Thanks to ##
 Naoaki Okazaki for his invaluable help in developing and distributing this toolkit
Yoshimasa Tsuruoka for the GENIA tagger
And other collegues who willingly tested NERsuite and gave comments and advices


## Compatibility ##
 * NERsuite has been built and tested on CentOS.
 * NERsuite needs a modified version of CRFsuite (https://github.com/priancho/crfsuite). 
 The original version is missing label bias functionality and will not work with this version of NERsuite.
 * NERsuite may not work on OSX (see also [#23](https://github.com/nlplab/nersuite/issues/23) reported by Florian Leitner).


## Citing ##

If you do make use of NERsuite or its components please cite as follows:

    @misc{,
        author       = {Cho, Han-Cheol and Okazaki, Naoaki and Miwa, Makoto 
                        and Tsujii, Jun'ichi},
        title        = {NERsuite: a named entity recognition toolkit},
        howpublished = {https://github.com/nlplab/nersuite},
    }


## History ##

Minor updates (2015.01.20)
  - New function: Added the support for CRFsuite label bias arg (by @spyssalo) by
                  using a customized CRFsuite (https://github.com/priancho/crfsuite).
                  Use "-b [Label name]:[Bias score],[Label name]:[Bias score],... ".


Minor updates (2012.07.06)
  - Bug fix: nersuite_gtagger crashed with segmentation fault when it runs without
              -multidoc argument.


Version 1.2.1 (2012.07.02)
  - Change: -multidoc [SEP] option for all programs now regards lines "beginning 
              with" [SEP] as comment lines and print them without any modification,
              whereas it previously dealt with the lines "exactly same" to [SEP].
              In addition, multiple comment lines can be used from this version.
  
  - Change: nersuite_tokenizer computes token offsets at document level, whereas
              it used sentence level token offsets. When it encounters a document
              separator, which is given by -multidoc [SEP] option, the tokenizer
              resets the offset to 0.
  
  - New function: correct_mp program which removes named entities having mismatched
              parenthesis is added. Documentation will be added to the homepage
              later.


Minor updates (2012.06.28)
  - Change: the standoff and brat output option now use unique IDs for entities 
              regardless of their semantic types. The IDs are also counted in 
              document level, whereas the conll option still counts it in sentence 
              level.

  - Bug fix: a bug of the string normalization part of the dictionary compiler and 
              tagger is fixed.
  
  - New function: brat format for output is added (by Sampo Pyysalo)


Version 1.2
  - All programs are refactored


Version 1.1
  - Now available to use external dictionaries. Dictionay compiler and tagger are
    included. (These two programs use CDB++ library)


Version 1.0
  - Initial release of NERsuite.
  - Statistical model only.
  
