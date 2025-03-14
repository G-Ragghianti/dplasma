#
# Copyright (c) 2012-2019 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
#
set(alltreel 0 1 2 3 )    # --treel Option for local tree
set(alltreeh 0 1 2 3 4 )  # --treeh Option for distributed tree
set(allP 3 5 7 8)         # --qr_p  Fix the size of the distributed tree
set(allA 1 2 4 7)         # --qr_a  Fix the size of the domain killed by TS
#set(allM 1 2 3 4 5 10 13 17 25 26 58 128)
#set(allN 1 2 3 4 5 10 13 17 25 26 58 128)
set(allM 1 3 4 10 17 25 128)
set(allN 1 2 5 13 26 58)

foreach( treel ${alltreel} )
  foreach( a ${allA} )
    foreach( m ${allM} )
      foreach( n ${allN} )
        add_test(pivgen_l${treel}_a${a}_m${m}_n${n} ./testing_dpivgen -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=1 )
        add_test(qrparam_l${treel}_a${a}_m${m}_n${n} ./testing_dgeqrf_hqr -t 1 -i 1 -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=1 )
        if(${a} GREATER 1)
          add_test(pivgen_l${treel}_a${a}_m${m}_n${n}_rr ./testing_dpivgen -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=1 --tsrr=1)
          add_test(qrparam_l${treel}_a${a}_m${m}_n${n}_rr ./testing_dgeqrf_hqr -t 1 -i 1 -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=1 --tsrr=1)
        endif()
      endforeach()
    endforeach()
  endforeach()

  foreach( domino 0 1 )
    foreach( treeh ${alltreeh} )
      foreach( p ${allP} )
        foreach( a ${allA} )
          foreach( m ${allM} )
            foreach( n ${allN} )
              add_test(pivgen_h${treeh}_p${p}_d${domino}_l${treel}_a${a}_m${m}_n${n} ./testing_dpivgen -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=${p} --treeh=${treeh} --domino=${domino})
              add_test(qrparam_h${treeh}_p${p}_d${domino}_l${treel}_a${a}_m${m}_n${n} ./testing_dgeqrf_hqr -t 1 -i 1 -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=${p} --treeh=${treeh} --domino=${domino})
              if(${a} GREATER 1)
                add_test(pivgen_h${treeh}_p${p}_d${domino}_l${treel}_a${a}_m${m}_n${n}_rr ./testing_dpivgen -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=${p} --treeh=${treeh} --domino=${domino} --tsrr=1)
                add_test(qrparam_h${treeh}_p${p}_d${domino}_l${treel}_a${a}_m${m}_n${n}_rr ./testing_dgeqrf_hqr -t 1 -i 1 -M ${m} -N ${n} --treel=${treel} --qr_a=${a} --qr_p=${p} --treeh=${treeh} --domino=${domino} --tsrr=1)
              endif()
            endforeach()
          endforeach()
        endforeach()
      endforeach()
    endforeach()
  endforeach()
endforeach()

set(allP 1 4 9 16 81)         # --qr_p  Fix the size of the distributed tree
set(allQ 2 4 9 16 81)         # --qr_p  Fix the size of the distributed tree
#set(allM 1 2 3 4 5 10 13 17 25 26 58 128)
#set(allN 1 2 3 4 5 10 13 17 25 26 58 128)
set(allM 3 4 10 17 25 128) #1 3 4 10 17 25 128)
set(allN 1 2 5 13 26 58)

#foreach( m ${allM} )
#  foreach( n ${allN} )
#    add_test(sys_pivgen_m${m}_n${n} ./testing_dpivgen -M ${m} -N ${n} --treel=0 --qr_a=1 --qr_p=1 )
#    add_test(sys_qrparam_m${m}_n${n} ./testing_dgeqrf_param -t 1 -i 1 -M ${m} -N ${n} --treel=0 --qr_a=1 --qr_p=1 )
#  endforeach()
#endforeach()

foreach( p ${allP} )
  foreach( q ${allQ} )
    foreach( m ${allM} )
      foreach( n ${allN} )
        add_test(sys_pivgen_p${p}_q${q}_m${m}_n${n}  ./testing_dpivgen                   -M ${m} -N ${n} --qr_a=${q} --qr_p=${p})
        add_test(sys_qrparam_p${p}_q${q}_m${m}_n${n} ./testing_dgeqrf_systolic -t 1 -i 1 -M ${m} -N ${n} --qr_a=${q} --qr_p=${p})
      endforeach()
    endforeach()
  endforeach()
endforeach()
