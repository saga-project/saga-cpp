#include "pages/development.h"
#include "pages/generator.h"
#include "pages/shared_data.h"
#include "pages/license.h"
#include "pages/packages.h"


/** @mainpage Table of Contents
* <p>
* CPI stands for <i>Capability Provider Interface</i> and it defines the interface
* between the SAGA runtime and adaptors (middleware plug-ins). 
* SAGA uses a dynamic loading mechanism that allows to load adaptors during 
* runtime to provide a specific functionality requested by the user 
* (see <a href="http://vimeo.com/20955440">http://vimeo.com/20955440</a>). 
* All SAGA adaptors have to be derived and conform with the CPI, otherwise the 
* dynamic loading mechanism will fail. 
* 
* The SAGA adaptor CPI provides a set of object-oriented abstract interfaces 
* that define the structure and semantics of a SAGA adaptor. Please refer to the
* <i>Adaptor Developer Documentation</i> below for further details. The C++ 
* class reference can be found on the left.
* </p>
*
* <p>
* \n
* <b>Adaptor Developer Documentation</b>:
* <hr>
* </p>
*
* <table border="0" width=100%><tr><td>
* - @ref development
*    - @ref dev_intro
*    - @ref dev_data
*        - @ref dev_data_instance
*        - @ref dev_data_adaptor
*    - @ref dev_exceptions
*    - @ref dev_logging
*    - @ref dev_troubleshooting
*    - @ref dev_bugs
*
*- @ref generator
*    - @ref generator_intro
*    - @ref generator_examples
*
*- @ref packages
*    - @ref package_advert
*    - @ref package_filesystem
*    - @ref package_job
*    - @ref package_replica
*    - @ref package_rpc
*    - @ref package_sd
*
*- @ref example
*    - @ref example_gen
*    - @ref example_fill
*    - @ref example_comp
* 
*- @ref licensing
*    - @ref licensing_considerations
*    - @ref licensing_boost
* </td><td>
* <p align="center"><img  src="./saga-cpi.png" /></p>
* </td></tr></table>
*
* <p>
* \n
* <b>Quick Class Reference Links: </b>
* <hr>
* </p>
*
* - Advert
*    - CPI Instance Data Members
*   
* - Filesystem
*    - CPI Instance Data Members
*   
* - Job
*    - CPI Instance Data Members
*   
*/

