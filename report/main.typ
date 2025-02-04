#set document(title: "MeMoCo - Lab Exercise - part 1 & 2", author: "Furno Francesco", date: datetime.today())
#set text(font: "New Computer Modern", lang: "en")
#set page(numbering: none)
#set heading(numbering: "1.1.")
#set enum(indent: 1em)
#set list(indent: 1em)
#show figure: set block(breakable: true)
#show link: underline
#show link: set text(fill: blue)

#align(center,
  [ 
    #line(length: 100%, stroke: gray + 0.5pt)
    #v(10pt)
    #text(strong("Lab exercise - Part I & II"), 24pt)\
    #v(6pt)
  #text(smallcaps("Methods and Models for Combinatorial Optimization"), 16pt)\
  #v(10pt)
  #line(length: 100%, stroke: gray + 0.5pt)
  #v(20pt)
  #image("img/unipd-logo.svg", width: 80%)
  #v(20pt)
  #line(length: 100%, stroke: gray + 0.5pt)
  #v(10pt)
  #text(smallcaps([Furno Francesco]), 20pt)
  #v(6pt)
  #text([francesco.furno\@studenti.unipd.it] + " - 2139507" , 14pt, style: "italic")
  
  #v(10pt)
  #line(length: 100%, stroke: gray + 0.5pt)
  #v(20pt)
  #text(style: "italic", 14pt, "A.A. 2024 - 2025")\
  ]
)
#pagebreak()
#[
  #show outline.entry.where(level: 1): it => {strong(it)}
  #outline(title: "Table of contents", indent: 1em)
]
#v(10pt)
#outline(
  title: "Figures",
  target: figure.where(kind: image)
)
#v(10pt)
#outline(
    title: "Tables",
    target: figure.where(kind: table),
    indent: auto
)

#pagebreak()
#set page(numbering: "1",
  header: [
    #align(right, [
      _Furno Francesco - 2139507_  #h(8pt)
    ])
  #line(length: 100%, stroke: gray + 0.5pt)
  ],
  footer: [
    #line(length: 100%, stroke: gray + 0.5pt)
    #grid(columns: 2, gutter: 30%, 
    align(right, [ #h(8pt)
      *Methods and Models for Combinatorial Optimization*
    ]),
    align(center, [
      #context counter(page).display()
    ])
  )
  ]
)

#include "chapters/1_Intro.typ"
#include "chapters/2_InstancesGen.typ"
#include "chapters/3_Cplex.typ"
#include "chapters/4_SA.typ"
#include "chapters/5_PC.typ"
#include "chapters/6_Conclusion.typ"