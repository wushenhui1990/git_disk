function AddTitle()
    call append(0,"/*******************************************************************************")
    call append(1,"     Copyright kaiyun. ".strftime("%Y"))
    call append(2,"     All Rights Reserved.")
    call append(3,"")
    call append(4,"     File: ".expand("%:t"))
    call append(5,"")
    call append(6,"     Description:")
    call append(7,"")
    call append(8,"     TIME LIST:")
    call append(9,"     Create by wsh   ".strftime("%Y-%m-%d %H:%M"))
    call append(10,"*******************************************************************************/")
    echohl WarningMsg | echo "Successful in adding the copyright." | echohl None
endfunction
