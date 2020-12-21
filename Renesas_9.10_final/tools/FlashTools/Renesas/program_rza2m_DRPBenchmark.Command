r
h

// Download loader into QSPI flash
loadbin .\Renesas\rza2m_sflash_boot_loader_gcc.bin,0x20000000
r
h

// Download application into QSPI flash
loadbin .\Renesas\RZA2M_DRP_Benchmark.bin,0x20010000
r
g
q
exit
