let s:AESENC_PassPhrase = ''
let s:AESENC_PassFilename = ''
let s:AESENC_Command = 'aesf'
function! s:AESENC_ReadPre()
	set cmdheight=1
	set viminfo=
	set nobackup
	set nowritebackup
	set noswapfile
	set shell=/bin/sh
	set bin
endfunction

function! s:AESENC_ReadPost()
	if line('$') == 1 && getline(1) == ''
		" Create the file
		while 1 == 1
			let curline = getline('.')
			call inputsave()
			echohl WarningMsg
			let l:PassPhrase1 = inputsecret('Enter passphrase: ')
			let l:PassPhrase2 = inputsecret('Confirm passphrase: ')
			echohl None
			call inputrestore()

			if  s:PassPhrase1 == ''
				exit
			elseif l:PassPhrase1 == l:PassPhrase2
				let s:PassPhrase = l:PassPhrase1
				let l:PassPhrase1 = ''
				let l:PassPhrase2 = ''

				let s:PassFilename = expand('%:p')

				if filereadable(s:PassFilename . '_') " Delete if temp file exists
					call delete(s:PassFilename . '_')
				endif

				bd
				execute 'e ' . s:PassFilename . '_'
				execute 'set syntax=markdown'
				autocmd VimEnter * echo 'File successfully created: ''' . s:PassFilename . '''.'

				break
			else
				autocmd VimEnter * echohl ErrorMsg
				autocmd VimEnter * echo 'Passwords do not match.'
				autocmd VimEnter * echohl none

				let l:PassPhrase1 = ''
				let l:PassPhrase2 = ''
			endif
		endwhile
	else
		" Ask for password
		let l:curline = getline('.')
		call inputsave()
		echohl WarningMsg
		let s:PassPhrase = inputsecret('Enter passphrase: ')
		echohl None
		call inputrestore()

		let s:PassFilename = expand('%:p')

		if filereadable(s:PassFilename . '_') " Delete if temp file exists
			call delete(s:PassFilename . '_')
		endif

		execute '!' . s:AESENC_Command . ' -p "' . s:PassPhrase . '" -f "' . s:PassFilename . '" -d > "' . s:PassFilename . '_"'

		if v:shell_error != 0
			call delete(s:PassFilename . '_')
			autocmd VimEnter * echohl ErrorMsg
			autocmd VimEnter * echo 'Unable to decrypt the file: ''' . s:PassFilename . '''.'
			autocmd VimEnter * echohl none
			bd
		else
			bd
			execute 'e ' . s:PassFilename . '_'
			execute 'set syntax=markdown'
			autocmd VimEnter * echo 'File successfully decrypted: ''' . s:PassFilename . '''.'
		endif
	endif
endfunction

function! s:AESENC_WritePost()
	silent execute '!' . s:AESENC_Command . ' -p "' . s:PassPhrase . '" -f "' . s:PassFilename . '_" -e > "' . s:PassFilename . '"'
	redraw | echo 'File encrypted to ''' . s:PassFilename . '''.'
endfunction

function! s:AESENC_Leave()
	call delete(s:PassFilename . '_')
endfunction

function! s:AESFRandom(length, charset)
    let l:output = system( s:AESENC_Command . ' -r ""' . a:charset . '"" ' . a:length )
    let l:output = substitute(l:output, '[\r\n]*$', '', '')
    execute 'normal a' . l:output
endfunction

function! s:Random()
	echohl WarningMsg
	let l:length = input('Enter the length of the string: ')
	echohl None

	redraw
	set cmdheight=2
	echo "List of charsets: alpha, numbers"
	echohl WarningMsg
	let l:charset = input('Enter the name of the charset: ', 'alpha')
	echohl None
	set cmdheight=1

	if l:charset == 'alpha'
		call s:AESFRandom(l:length, 'abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMOPQRSTUVWXYZ0123456789')
	elseif l:charset == 'numbers'
		call s:AESFRandom(l:length, '0123456789')
	else
		redraw
		echohl ErrorMsg
		echo 'Unable to find charset: ''' . l:charset . '''.'
		echohl none
	endif
endfunction

autocmd BufNewFile,BufReadPre,FileReadPre *.aesf call s:AESENC_ReadPre()
autocmd BufNewFile,BufReadPost,FileReadPost *.aesf call s:AESENC_ReadPost()
autocmd BufWritePost,FileWritePost *.aesf_ call s:AESENC_WritePost()
autocmd BufLeave,VimLeave, *.aesf_ call s:AESENC_Leave()

command! Random call s:Random()
