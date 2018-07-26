function Xc_fun()
set filetype=xc
nnoremap <Leader>g yiw:grep! <C-r>" <C-R>=expand('%:p:h') . '/*'<CR> --exclude-dir=.build -nr
nnoremap <Leader>G yiwq:igrep! <C-r>" * --exclude-dir=.build -nr <ESC>
vnoremap <Leader>g yq:igrep! "<C-r>"" <C-R>=expand('%:p:h') . '/*'<CR> --exclude-dir=.build -nr
vnoremap <Leader>G yq:igrep! "<C-r>"" * --exclude-dir=.build -nr<ESC>
endfunction

