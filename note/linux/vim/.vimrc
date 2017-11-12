set autoindent
set tabstop=4
set nu
set cindent
set shiftwidth=4
set fileencodings=utf-8,gb18030
set incsearch "highlight search, realtime search
set hlsearch "keep highlight search ,:nohl to mute


nnoremap <silent> [b :bprevious<CR>
nnoremap <silent> ]b :bnext<CR>
nnoremap <silent> [B :bfirst<CR>
nnoremap <silent> ]B :blast<CR>

nnoremap <silent> [q :cprevious<CR>
nnoremap <silent> ]q :cnext<CR>
nnoremap <silent> [Q :cfirst<CR>
nnoremap <silent> ]Q :clast<CR>
