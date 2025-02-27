AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 30
   name "MusicTrackID"
   tl -60.417 12.417
   children {
    33
   }
   valueMax 100
  }
  IOPItemInputClass {
   id 31
   name "BroadcastType"
   tl -88.417 -194.333
   children {
    37
   }
  }
  IOPItemInputClass {
   id 32
   name "Offset"
   tl -64.417 -93.583
   children {
    36
   }
   valueMax 300000
  }
  IOPItemInputClass {
   id 35
   name "DJTrackID"
   tl -70.417 124.417
   children {
    34
   }
  }
  IOPItemInputClass {
   id 41
   name "RadioVolume"
   tl -71 220
   children {
    40
   }
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 33
   name "MusicTrackID"
   tl 188.583 28.417
   input 30
  }
  IOPItemOutputClass {
   id 34
   name "DJTrackID"
   tl 189.583 124.417
   input 35
  }
  IOPItemOutputClass {
   id 36
   name "Offset"
   tl 180.583 -91.583
   input 32
  }
  IOPItemOutputClass {
   id 37
   name "BroadcastType"
   tl 181.583 -194.333
   input 31
  }
  IOPItemOutputClass {
   id 40
   name "RadioVolume"
   tl 167 221
   input 41
  }
 }
 compiled IOPCompiledClass {
  visited {
   517 518 389 134 261 262 133 390 5 6
  }
  ins {
   IOPCompiledIn {
    data {
     1 2
    }
   }
   IOPCompiledIn {
    data {
     1 196610
    }
   }
   IOPCompiledIn {
    data {
     1 131074
    }
   }
   IOPCompiledIn {
    data {
     1 65538
    }
   }
   IOPCompiledIn {
    data {
     1 262146
    }
   }
  }
  outs {
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
  }
  processed 10
  version 2
 }
}