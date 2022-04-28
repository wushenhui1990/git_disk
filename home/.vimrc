"vundle setting
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

Plugin 'junegunn/vim-easy-align'
" Keep Plugin commands between vundle#begin/end.
Plugin 'davidhalter/jedi-vim'

Plugin 'scrooloose/nerdcommenter'

Plugin 'vhda/verilog_systemverilog.vim'
"Plugin 'Valloric/YouCompleteMe'

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
"

set backspace=indent,eol,start
set tabstop=4
set shiftwidth=4
set nu
set sc "show selected line num
set expandtab "tab turn to space
syntax on
"set cindent
set autoindent
set noro
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
set complete=.,s,b,u,i "complete only scan the current buffer, see help 'complete'
set wrapscan
"set wildmode=longest "tab complete filename
set pastetoggle=<F9>
set conceallevel=0 "let vim not auto convert -> to chinese view
"set grepprg=grep\ --color=always\ -n\ $*\ /dev/null
let g:netrw_liststyle=1
let Tlist_WinWidth=50
let Tlist_Show_One_File=1
let Tlist_Enable_Fold_Column=0
let g:ycm_enable_diagnostic_signs=1
let g:ycm_show_diagnostics_ui=1
let g:ycm_autoclose_preview_window_after_completion=1
set updatetime=1000
colo desert
au BufLeave * silent! wa
set autowrite
"yank to system clipboard,request "vim --version +clipboard or +xterm_clipboard
set clipboard=unnamed

set foldmethod=syntax "verilog_systemverilog.vim used
runtime macros/matchit.vim "verilog syntax

so ~/.vim/functions/add_title.vim
nnoremap <silent> <F4> :call AddTitle()<CR>


" Start interactive EasyAlign in visual mode (e.g. vipga)
xmap <Leader>a <Plug>(EasyAlign)

" Start interactive EasyAlign for a motion/text object (e.g. gaip)
nmap <Leader>a <Plug>(EasyAlign)
nnoremap <silent> <F7> :cw<CR>
nnoremap <silent> <F8> :TlistOpen<CR>
nnoremap <silent> <F5> :!ctags --c-kinds=+px --langmap=c:+.xc -R<CR>

nnoremap <silent> [b :bprevious<CR>
nnoremap <silent> ]b :bnext<CR>
nnoremap <silent> [B :bfirst<CR>
nnoremap <silent> ]B :blast<CR>

nnoremap <silent> [q :cprevious<CR>
nnoremap <silent> ]q :cnext<CR>
nnoremap <silent> [Q :cfirst<CR>
nnoremap <silent> ]Q :clast<CR>
nnoremap <silent> ]Q :clast<CR>
nnoremap <Leader>b :b #<CR>
nnoremap <S-tab> :tabnex<CR> 
"nnoremap <tab> :tabnex<CR> 

nnoremap <Leader>g yiw:grep! <C-r>" <C-R>=expand('%:p:h') . '/*'<CR> --exclude=tags -Inr
nnoremap <Leader>G yiwq:igrep! <C-r>" * -Inr<ESC>
vnoremap <Leader>g y:grep! <C-r>" <C-R>=expand('%:p:h') . '/*'<CR> --exclude=tags -Inr
vnoremap <Leader>G yq:igrep! "<C-r>"" * -Inr<ESC>

nnoremap <Leader>t <C-w>hyiw<C-w>l:grep! <C-r>" <C-R>=expand('%:p:h') . '/*'<CR> -Inr
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

"reselect last visual select
nnoremap <Leader>s `>V`<

nnoremap <C-n> :nohl<CR>
nnoremap <C-l> :ls<CR>:b 
nnoremap <C-g> 1<C-g> "show full path
nnoremap <C-p> diw"0P
nnoremap <C-]> <C-]>zz

"yank select char to null and past
xnoremap p "_dP 
"inseart list
nnoremap <Leader>l :put=map(range(0,100),'printf(''/*0x%04x*/'',v:val)')
nnoremap <Leader>e :e <C-R>=expand('%:p:h') . '/'<CR>
nnoremap <Leader>n :grep! "<C-r>=expand('%:t')<CR>" <C-R>=expand('%:p:h') . '/*'<CR> -nr
nnoremap SQ <ESC>:mksession! .vim_session<CR>:wqa<CR>


inoremap +pk <ESC>:read ~/.vim/template/debug_printk<CR>
inoremap +pf <ESC>:read ~/.vim/template/debug_printf<CR>
inoremap +t <ESC>:pu=strftime('%Y/%m/%d %T')<CR>
if &diff
	:syntax off
endif

so ~/.vim/functions/xc_fun.vim
autocmd BufRead,BufNewFile *.xc  call Xc_fun() "add file type detect, syntax file is in ~/.vim/syntax; rf :h new-filetype , :h syntax
autocmd BufRead,BufNewFile *.xml  nnoremap <Leader>c :F<a!--<ESC>f>i--<ESC>
autocmd BufRead,BufNewFile *.xn  nnoremap <Leader>c :F<a!--<ESC>f>i--<ESC>
autocmd BufRead,BufNewFile *.sh  set noexpandtab
"au FilterWritePre * if &diff | colorscheme my_color | endif
"
let g:NERDCustomDelimiters = { 'asm': { 'left': '/**','right': '*/' } }

let g:jedi#goto_assignments_command = "gD"
let g:jedi#rename_command = ''
let g:jedi#goto_command = "<C-]>"
