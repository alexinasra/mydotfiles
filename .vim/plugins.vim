let s:plugin_dir = expand('~/.vim/plugged')

function! s:ensure(repo)
  let name = split(a:repo, '/')[-1]
  let path = s:plugin_dir . '/' . name

  if !isdirectory(path)
    if !isdirectory(s:plugin_dir)
      call mkdir(s:plugin_dir, 'p')
    endif
    execute '!git clone --depth=1 https://github.com/' . a:repo . ' ' . shellescape(path)
  endif

  execute 'set runtimepath+=' . fnameescape(path)
endfunction

call s:ensure('ghifarit53/tokyonight-vim')
call s:ensure('junegunn/fzf')
call s:ensure('junegunn/fzf.vim')
call s:ensure('itchyny/lightline.vim')
call s:ensure('yegappan/lsp')
call s:ensure('rhysd/vim-clang-format')


" --- Fix Netrw Update Bugs ---
" 1. Fixes the Netrw Option Restore bug when changing directories
let g:netrw_returntodir = 0
let g:netrw_fastbrowse  = 0


augroup UltraFixNetrw
  autocmd!
  " Force Vim to forcefully drop the lock while Netrw processes state updates
  autocmd FileType netrw setlocal modifiable
  " Instantly lock it right back down the millisecond Netrw finishes loading
  autocmd BufWinEnter * if &ft ==# 'netrw' | setlocal nomodifiable noswapfile | endif
augroup END



