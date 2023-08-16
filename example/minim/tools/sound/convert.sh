for f in $(ls *.BIN); do cp head.bin convert/$f; cat swab/$f >> convert/$f; done

