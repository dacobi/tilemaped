for f in $(ls *.BIN); do dd if=$f of=swab/$f conv=swab; done

