" Set leader key
let mapleader = " "

" Open netrw with <leader>cd
nnoremap <leader>cd :Ex<CR>


"Clang-format
autocmd FileType c,cpp,objc,java,javascript,typescript,proto nnoremap <buffer><Leader>cf :<C-u>ClangFormat<CR>
autocmd FileType c,cpp,objc,java,javascript,typescript,proto vnoremap <buffer><Leader>cf :ClangFormat<CR>

let g:clang_format#auto_format = 1

