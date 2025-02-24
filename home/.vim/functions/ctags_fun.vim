function Ctags_fun()
   if filereadable("generate_ctags.sh")
        !./generate_ctags.sh
   else
        !ctags --c-kinds=+px --langmap=c:+.xc -R
   endif
endfunction
