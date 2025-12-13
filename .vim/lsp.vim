" Enable diagnostics highlighting
let lspOpts = #{autoHighlightDiags: v:true}
autocmd User LspSetup call LspOptionsSet(lspOpts)
let lspServers = [
    \ #{
    \   name: 'rust-analyzer',
    \   filetype: ['rust'],
    \   path: 'rust-analyzer',
    \   args: []
    \ },
    \ #{
    \   name: 'bash',
    \   path: 'bash-language-server',
    \   args: [ 'start' ],
    \   filetype: [ 'sh' ]
    \ },
    \ #{
    \   name: 'clangd',
    \   filetype: ['c', 'cpp'],
    \   path: 'clangd',
    \   args: ['--background-index', '--clang-tidy']
    \ },
    \ #{
    \   name: 'cssls',
    \   filetype: ['css'],
    \   path: 'css-languageserver',
    \   args: ['--stdio'],
    \ }
\ ]

autocmd User LspSetup call LspAddServer(lspServers)

" Key mappings
nnoremap gd :LspGotoDefinition<CR>
nnoremap gr :LspShowReferences<CR>
nnoremap K  :LspHover<CR>
nnoremap gl :LspDiag current<CR>
nnoremap <leader>nd :LspDiag next \| LspDiag current<CR>
nnoremap <leader>pd :LspDiag prev \| LspDiag current<CR>
inoremap <silent> <C-Space> <C-x><C-o>

" Set omnifunc for completion
autocmd FileType rust setlocal omnifunc=lsp#complete

" Custom diagnostic sign characters
autocmd User LspSetup call LspOptionsSet(#{
    \   diagSignErrorText: '✘',
    \   diagSignWarningText: '▲',
    \   diagSignInfoText: '»',
    \   diagSignHintText: '⚑',
    \ })
