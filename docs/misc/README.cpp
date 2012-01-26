
Notes about SAGA C++ language bindings
======================================

  Problem:  Flags on API level:need to cast to Flag, or use int.
  Reason:   cpp converts  (Flag | Flag) to int
  Solution: use ints (no casting needed, val check on runtime in engine)

  Problem:  enums are all supposed to include {Unknown = -1} - that leads to
            conflicts (multiply defined)
  Reason:   saga::file::Unknown == saga::directory::Unknown
  Solution: define unknown once, globally

