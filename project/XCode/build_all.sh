# This script builds all available saga components (engine/packages/adaptors)
# using XCode's command line project builder. It's just for convenince ;-)
#
#!/bin/bash

## The SAGA Engine + Packages
/usr/bin/xcodebuild -project saga_engine.xcodeproj -configuration Debug \
                    -target ALL

## The default adaptors
/usr/bin/xcodebuild -project adaptors/saga_default_adaptors.xcodeproj \
                    -configuration Debug -target ALL
                    
## The Globus preWS adaptors
/usr/bin/xcodebuild -project adaptors/saga_globus4-preWS_adaptors.xcodeproj \
                    -configuration Debug -target ALL
                    
## The OMII gridSAM adaptors
#/usr/bin/xcodebuild -project adaptors/saga_OMII_adaptors.xcodeproj \
#                    -configuration Debug -target ALL
                    
