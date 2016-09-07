.section .data
  outputIntro: .asciz "\n***Made by Bruno Emori (RA: 88736) & Alessandra Iriguti (RA: 88741) - UEM 2016***\n"
  outputEnqueue: .asciz "\n**Enqueue**:\n"
  outputDequeue: .asciz "\n**Dequeue**:\n"
	outputConsult: .asciz "\n**Consult**:\n"
	outputChange: .asciz "\n**Change Registry:**\n"
  outputQueue: .asciz "\nQueue registries:"
  outputMenu: .asciz "\nChoose an option:\n(1) - Enqueue\n(2) - Dequeue\n(3) - Consult\n(4) - Change registry\n(5) - List\n(6) - Exit\n"
  outputExit: .asciz "\nProgram ended.\n"
  outputName: .asciz "\nName: %s"
  outputBirthdateDay: .asciz "\nBirthdate (Day): %d"
  outputBirthdateMonth: .asciz "\nBirthdate (Month): %d"
  outputBirthdateYear: .asciz "\nBirthdate (Year): %d"
  outputSex: .asciz "\nSex: %c"
  outputOccupation: .asciz "\nOccupation: %s"
  outputWage: .asciz "\nWage: %.2lf\n\n"
  
  msgError: .asciz "\nInvalid option.\n"
  msgEmpty: .asciz "\nThe queue is empty.\n\n"
  msgRemove: .asciz "\nRegistry removed.\n"
  msgInsert: .asciz "\nRegistry inserted.\n\n"
	msgTest: .asciz "\n\nThis is a test message.\n\n"
	msgNotFound: .asciz "\nName (%s) not found.\n"

  inputName: .asciz "\nName: "
  inputBirthdateDay: .asciz "\nBirthdate (Day): "
  inputBirthdateMonth: .asciz "\nBirthdate (Month): "
  inputBirthdateYear: .asciz "\nBirthdate (Year): "
  inputSex: .asciz "\nSex: "
  inputOccupation :.asciz "\nOccupation: "
  inputWage: .asciz "\nWage: "

  typeString: .asciz "%s"
  typeChar: .asciz "%c"
  typeDec: .asciz "%d"
	typeFloat: .asciz "%lf"

  NULL: .int 0
  menuOption: .int 0
  pushC: .space 4

  #Variables used in registries: 
  Name: .space 44
  Day: .space 4
  Month: .space 4
  Year: .space 4
  Sex: .space 4 #M or F
  Occupation: .space 24
  Wage: .space 8
  Next: .int NULL

  nMemoryAlloc: .int 96
  queuePointer: .int NULL
  lastPointer: .int NULL
  regPointer: .int NULL
	auxPointer: .int NULL
	auxPointer2: .int NULL

.section .text
.globl main

main:
  jmp main2

readData:
  pushl %edi

  pushl $inputName
  call printf
  addl $4, %esp
  pushl $typeString
  call scanf
  addl $4, %esp

  popl %edi
  addl $44, %edi
  pushl %edi

  pushl $typeChar
  call scanf 
  addl $4, %esp
  
  pushl $inputBirthdateDay
  call printf
  addl $4, %esp
  pushl $typeDec
  call scanf
  addl $4, %esp
  
  popl %edi
  addl $4, %edi
  pushl %edi

  pushl $typeChar
  call scanf 
  addl $4, %esp

  pushl $inputBirthdateMonth
  call printf
  addl $4, %esp
  pushl $typeDec
  call scanf
  addl $4, %esp

  popl %edi
  addl $4, %edi
  pushl %edi

  pushl $typeChar
  call scanf 
  addl $4, %esp

  pushl $inputBirthdateYear
  call printf
  addl $4, %esp
  pushl $typeDec
  call scanf
  addl $4, %esp

  popl %edi
  addl $4, %edi
  pushl %edi

  pushl $typeChar
  call scanf 
  addl $4, %esp

  pushl $inputSex
  call printf
  addl $4, %esp
  pushl $typeChar
  call scanf
  addl $4, %esp

  popl %edi
  addl $4, %edi
  pushl %edi

  pushl $typeChar
  call scanf 
  addl $4, %esp

  pushl $inputOccupation
  call printf
	addl $4, %esp
  pushl $typeString
  call scanf
  addl $4, %esp

  popl %edi
  addl $24, %edi
  pushl %edi

  pushl $inputWage
  call printf
	addl $4, %esp
  pushl $typeFloat
  call scanf
  addl $4, %esp 
  
  popl %edi
  addl $8, %edi
  
  movl $NULL, (%edi)
  subl $92, %edi
  
  RET

showData:
  pushl %edi

  pushl $outputName
  call printf
  addl $4, %esp

  popl %edi
  addl $44, %edi
  pushl %edi
    
  pushl (%edi)
  pushl $outputBirthdateDay
  call printf
  addl $8, %esp

  popl %edi
  addl $4, %edi
  pushl %edi

  pushl (%edi)
  pushl $outputBirthdateMonth
  call printf
  addl $8, %esp

  popl %edi
  addl $4, %edi
  pushl %edi

  pushl (%edi)
  pushl $outputBirthdateYear
  call printf
  addl $8, %esp

  popl %edi
  addl $4, %edi
  pushl %edi

  pushl (%edi)
  pushl $outputSex
  call printf
  addl $8, %esp

  popl %edi
  addl $4, %edi
  pushl %edi

  pushl $outputOccupation
  call printf
  addl $4, %esp

  popl %edi
  addl $24, %edi

	fldl (%edi)
	subl $8, %esp
	fstl (%esp)	
  pushl $outputWage
  call printf
	addl $12, %esp

  subl $84, %edi

  RET

dequeue:
	pushl $inputName
	call printf
	addl $4, %esp
		
	pushl $Name
	pushl $typeString
	call scanf
	addl $8, %esp

	pushl $Name
	movl queuePointer, %edi
	movl %edi, auxPointer

continueDequeue:	
	cmpl $NULL, %edi
	jz notFound

	pushl %edi
	call strcmp
	addl $4, %esp

	cmpl $0, %eax
	jz removeNameFound
	
	movl %edi, auxPointer
	movl 92(%edi), %edi
	jmp continueDequeue

removeNameFound:
	movl auxPointer, %esi
	cmpl auxPointer, %edi
	jz removeFirst

	movl 92(%edi), %eax
	movl %eax, 92(%esi)
	
	pushl %edi
	call free
	addl $8, %esp
  ret

removeFirst:
	movl 92(%esi), %edi
	movl %edi, queuePointer
	pushl %esi
	call free
	addl $8, %esp
  ret

showQueue:
  movl queuePointer, %edi
  cmpl $NULL, %edi
  jnz continueShowQueue

  pushl $msgEmpty
  call printf
  addl $4, %esp

  jmp menuList

continueShowQueue:
  movl queuePointer, %edi

return:
  cmpl $NULL, %edi
  jz callReturn

	pushl %edi
	call showData
	addl $4, %esp
	
  movl 92(%edi), %edi

  jmp return

callReturn:
  ret

enqueueFirst:
	movl queuePointer, %eax
  movl %edi, queuePointer
	movl %eax, 92(%edi)
  movl %edi, lastPointer
	pushl $msgInsert
	call printf
	addl $4, %esp
  ret

enqueue:
  movl nMemoryAlloc, %ecx
  pushl %ecx
  call malloc
  addl $4, %esp

  movl %eax, regPointer
  movl regPointer, %edi
  call readData

  cmpl $NULL, queuePointer
  jz enqueueFirst
	jmp inOrderEnqueue

inOrderEnqueue:
	movl queuePointer, %esi
	movl %esi, auxPointer
	pushl %edi

continueInOrderEnqueue:
	pushl %esi
	call strcmp
	addl $4, %esp

	cmpl $0, %eax
	jge continueEnqueue

	cmpl $NULL, 92(%esi)
	je lastEnqueue

	movl %esi, auxPointer
	movl 92(%esi), %esi

  jmp continueInOrderEnqueue

lastEnqueue:
	movl %edi, 92(%esi)
  addl $4, %esp
  pushl $msgInsert
  call printf
  addl $4, %esp
  ret

continueEnqueue:
  addl $4, %esp
	cmpl auxPointer, %esi
	je enqueueFirst

	movl auxPointer, %eax
	movl %esi, 92(%edi)
	movl %edi, 92(%eax)
  ret

consultQueue:
	pushl $inputName
	call printf
	addl $4, %esp
		
	pushl $Name
	pushl $typeString
	call scanf
	addl $8, %esp

	pushl $Name
	movl queuePointer, %edi

continueConsult:	
	cmpl $NULL, %edi
	jz notFound

	pushl %edi
	call strcmp

	addl $4, %esp
	cmpl $0, %eax
	jz nameFound

	movl 92(%edi), %edi
	jmp continueConsult

nameFound:
	pushl %edi
	call showData
	addl $8, %esp
  ret

notFound:
	pushl $Name
	pushl $msgNotFound
	call printf
	addl $12, %esp
  ret

endProgram:
  pushl $outputExit
  call printf
  pushl $0
  call exit

menuList:
  pushl $outputMenu
  call printf

  pushl $menuOption
  pushl $typeDec
  call scanf
  
  addl $12, %esp
 
  pushl $pushC
  pushl $typeChar
  call scanf
  addl $8, %esp

  cmpl $1, menuOption
  jz insertFunction

  cmpl $2, menuOption
  jz removeFunction
	
  cmpl $3, menuOption
  jz consultFunction
  
  cmpl $4, menuOption
  jz changeFunction
	
  cmpl $5, menuOption
  jz listFunction
  
  cmpl $6, menuOption
  jz endProgram

removeFunction:
  pushl $outputDequeue
  call printf
  addl $4, %esp
  call dequeue
  jmp menuList

insertFunction:
  pushl $outputEnqueue
  call printf
  addl $4, %esp
  call enqueue  
  jmp menuList

consultFunction:
	pushl $outputConsult
	call printf
	addl $4, %esp
  call consultQueue
  jmp menuList

listFunction:
  call showQueue
  jmp menuList

changeFunction:
  pushl $outputChange
  call printf
  addl $4, %esp
  call dequeue
  call enqueue
  jmp menuList

main2:
  pushl $outputIntro
  call printf
  jmp menuList
