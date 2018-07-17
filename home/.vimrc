set tabstop=4
set shiftwidth=4
set nu
syntax on
set cindent
set fileencodings=utf-8,gb18030
set fileformats=unix,dos
set incsearch "highlight search, realtime search
set hlsearch "keep highlight search ,:nohl to mute
set laststatus=2
set statusline=%F "let status line show full path
set statusline+=%m "modify flag
set hidden "keep undolist 
set undodir=~/.vim/undo
set tags=tags
set tags+=~/.vim/tags
set tags+=my_tags
set wrapscan
set pastetoggle=<F9>
set conceallevel=0 "let vim not auto convert -> to chinese view
"set grepprg=grep\ --color=always\ -n\ $*\ /dev/null
let g:netrw_liststyle=1
let Tlist_WinWidth=40
let Tlist_Show_One_File=1
let g:ycm_enable_diagnostic_signs=0
let g:ycm_show_diagnostics_ui=0
let g:ycm_autoclose_preview_window_after_completion=1
set updatetime=1000
colo desert
au FocusLost * silent! wa
set autowrite
"yank to system clipboard,request "vim --version +clipboard or +xterm_clipboard
set clipboard=unnamed

so ~/.vim/functions/add_title.vim
nnoremap <silent> <F4> :call AddTitle()<CR>

nnoremap <silent> <F7> :cw<CR>
nnoremap <silent> <F8> :TlistOpen<CR>
nnoremap <silent> <F5> :!ctags -R<CR>

nnoremap <silent> [b :bprevious<CR>
nnoremap <silent> ]b :bnext<CR>
nnoremap <silent> [B :bfirst<CR>
nnoremap <silent> ]B :blast<CR>

nnoremap <silent> [q :cprevious<CR>
nnoremap <silent> ]q :cnext<CR>
nnoremap <silent> [Q :cfirst<CR>
nnoremap <silent> ]Q :clast<CR>
nnoremap <silent> ]Q :clast<CR>
nnoremap <C-n> :nohl<CR>
nnoremap <Leader>b :b #<CR>
nnoremap <C-l> :ls<CR>:b 
nnoremap <S-tab> :tabnex<CR> 
"nnoremap <tab> :tabnex<CR> 

nnoremap <Leader>g yiw:grep! <C-r>" <C-R>=expand('%:p:h') . '/*'<CR> -nr
nnoremap <Leader>G yiwq:igrep! <C-r>" * -nr<ESC>
vnoremap <Leader>g yq:igrep! "<C-r>"" <C-R>=expand('%:p:h') . '/*'<CR> -nr
vnoremap <Leader>G yq:igrep! "<C-r>"" * -nr<ESC>

nnoremap <Leader>t <C-w>hyiw<C-w>l:grep! <C-r>" <C-R>=expand('%:p:h') . '/*'<CR> -nr
nnoremap <silent> <C-]> g<C-]>
nnoremap <Leader>f [[b%b
nnoremap <Leader>z [{v%zf
nnoremap <Leader>r "9yiw:,$s/<C-r>9/
vnoremap <Leader>r "9y:,$s/<C-r>9/
nnoremap <Leader>R "9yiw:%s/<C-r>9/
vnoremap <Leader>R "9y:%s/<C-r>9/

nnoremap == gg=G``zz
nnoremap <Leader>q <C-w>j<C-w>l<C-w>q
"nnoremap ## I#<ESC>
nnoremap <Leader>d :bprevious<CR>:bdelete #<CR>
nnoremap [i :[I "show all tags
nnoremap <C-g> 1<C-g> "show full path
nnoremap <C-p> diw"0P
nnoremap <C-]> <C-]>zz

"reselect last visual select
nnoremap <Leader>s `>V`<

"yank select char to null and past
xnoremap p "_dP 

nnoremap <Leader>e :e <C-R>=expand('%:p:h') . '/'<CR>
nnoremap <Leader>n :grep! "<C-r>=expand('%:t')<CR>" <C-R>=expand('%:p:h') . '/*'<CR> -nr
nnoremap SQ <ESC>:mksession! .vim_session<CR>:wqa<CR>


inoremap +pk <ESC>:read ~/.vim/template/debug_printk<CR>
inoremap +pf <ESC>:read ~/.vim/template/debug_printf<CR>
inoremap +t <ESC>:pu=strftime('%Y/%m/%d %T')<CR>
if &diff
	:syntax off
endif

autocmd BufRead,BufNewFile *.xc set filetype=xc "add file type detect, syntax file is in ~/.vim/syntax; rf :h new-filetype , :h syntax
"au FilterWritePre * if &diff | colorscheme my_color | endif
