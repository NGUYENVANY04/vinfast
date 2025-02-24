"    __   __(_)___ ___  __________
"    | | / / / __ `__ \/ ___/ ___/
"   _| |/ / / / / / / / /  / /__
"  (_)___/_/_/ /_/ /_/_/   \___/

"----------------------------------------------------------------
" Version : 1.0.0 (Modified for C Programming)
" License : NGUYEN VAN Y
"----------------------------------------------------------------

" Change background and text color of suggestion menu
hi CocFloating ctermbg=Black ctermfg=White
hi CocMenuSel ctermbg=DarkBlue ctermfg=White
hi CocSearch ctermfg=Yellow

" Set theme
let g:lightline = {'colorscheme' : 'tokyonight'}
let g:airline_theme = "tokyonight"
let g:tokyonight_style = 'night'
let g:tokyonight_enable_italic = 1
set termguicolors
syntax enable
colorscheme tokyonight

" Cursor settings
let &t_SI = "\e[5 q"
let &t_EI = "\e[2 q"

" Set the home directory for Coc.nvim configuration
let g:coc_config_home = "~/.config/vem"

" General Settings
set nocompatible
set backspace=indent,eol,start
set encoding=utf-8
if has("gui_running")
    set guifont=DejaVu\ Sans\ Mono:h12
endif
set showcmd
set showmode
set hidden
set history=1000
set ruler
set signcolumn=no
set number
set relativenumber
filetype plugin on

" UI Settings
set background=dark

" C Programming Settings
filetype plugin indent on
autocmd FileType c,cpp setlocal tabstop=4 shiftwidth=4 expandtab autoindent smartindent

" Floating terminal settings
let g:floaterm_width = 0.8
let g:floaterm_height = 0.7
let g:floaterm_autoclose = 2

" Search Settings
set ignorecase
set smartcase
set incsearch
set hlsearch
set clipboard=unnamedplus

" File Backup Settings
set nobackup
set nowritebackup
set noswapfile

" Plugin Manager: vim-plug
call plug#begin('~/.vim/plugged')
Plug 'preservim/nerdtree'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'preservim/nerdcommenter'
Plug 'jiangmiao/auto-pairs'
Plug 'vim-syntastic/syntastic'
Plug 'Yggdroot/indentLine'
Plug 'majutsushi/tagbar'
Plug 'easymotion/vim-easymotion'
Plug 'voldikss/vim-floaterm'
Plug 'rhysd/vim-clang-format'
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'ghifarit53/tokyonight-vim'
call plug#end()

" Plugin Configuration
nmap <C-b> :NERDTreeToggle<CR>
nmap <C-j> :w<CR> :FloatermToggle<CR>
imap <C-j> <Esc> :w<CR> :FloatermToggle<CR>
let g:airline#extensions#tabline#enabled = 1
let g:NERDCreateDefaultMappings = 1
let g:NERDTreeShowHidden=1
let g:AutoPairsMapBS = 1
let g:syntastic_enable_signs=1
let g:indentLine_enabled = 1
map <Leader><Leader>w <Plug>(easymotion-w)

" Shortcut Mappings
inoremap <C-s> <Esc>:ClangFormat<CR> :w<CR> a
nnoremap <C-s> :ClangFormat<CR> :w<CR>
inoremap <C-x> <Esc>:w <CR> <C-w>w
nnoremap <C-x> <C-w>w
inoremap <C-z> <Esc>u i
nnoremap <C-a> ggVGy
tnoremap <C-\> <C-\><C-n>

" Configure clang-format options
let g:clang_format#style_options = {
  \ "BasedOnStyle": "WebKit",
  \ "ColumnLimit": 100,
  \ "PointerBindsToType": v:false,
  \ "BreakBeforeBinaryOperators": "NonAssignment",
  \ "BreakBeforeBraces": "Allman",
  \ "AlignAfterOpenBracket": v:true,
  \ "SortIncludes": v:false,
  \ }

" Autocompletion settings
inoremap <silent><expr> <TAB>
      \ pumvisible() ? "\<C-n>" :
      \ <SID>check_back_space() ? "\<TAB>" :
      \ coc#refresh()
inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"
function! s:check_back_space() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction
if has('nvim')
  inoremap <silent><expr> <c-space> coc#refresh()
else
  inoremap <silent><expr> <c-@> coc#refresh()
endif
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm() : "\<CR>"
autocmd CursorHold * silent call CocActionAsync('highlight')
set statusline^=%{coc#status()}%{get(b:,'coc_current_function','')}
tnoremap <ESC><ESC> <C-\><C-n>:FloatermToggle<CR>
nnoremap <silent> <s-tab> :if &modifiable && !&readonly && &modified <CR> :write<CR> :endif<CR>:bnext<CR>
set updatetime=300
nmap <F3> :TagbarToggle<CR>
let g:tagbar_width = 30
