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
   tl -62.417 -193.333
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
   tl -58.417 124.417
   children {
    34
   }
  }
  IOPItemInputClass {
   id 41
   name "RadioVolume"
   tl -57 219
   children {
    40
   }
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 33
   name "MusicTrackID"
   tl 185.583 12.417
   input 30
  }
  IOPItemOutputClass {
   id 34
   name "DJTrackID"
   tl 185.333 124.792
   input 35
  }
  IOPItemOutputClass {
   id 36
   name "Offset"
   tl 185.833 -92.833
   input 32
  }
  IOPItemOutputClass {
   id 37
   name "BroadcastType"
   tl 181.833 -192.583
   input 31
  }
  IOPItemOutputClass {
   id 40
   name "RadioVolume"
   tl 184 219
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