#let tab-colors = (
  pari: white,
  dispari: luma(215),
  head: black.lighten(20%)
)
#let h_cell(t) = (table.cell(fill: tab-colors.head, text(fill: white,  weight: "bold", t)))



#let tests_cplex(path) = {
  let dataset = csv(path).flatten()
  dataset.at(0) = raw(dataset.at(0))
  let value = 6
  //set name file to raw
  while value < (dataset.len() -1){
    if calc.rem(value, 6) == 0 {
      dataset.at(value) = raw(dataset.at(value))
    }
    value = value + 1
  }

 

  align(center,
  table(
    align: center,
    table.header(h_cell("Dataset"), h_cell("Problem size"), h_cell("Time limit"), h_cell("Solving Time"),  h_cell("Status"), h_cell("Objetive value")),
    columns: 6,
    inset: 0.8em,
    fill: (_, row) => 
      if calc.odd(row) {
        tab-colors.dispari
      } else {tab-colors.pari},
    ..dataset
    )
  )
}

#let cplex_score(path) = {
  let dataset = csv(path).flatten()
  let avg_data = ()
  let num = 9
  let sizes = (10, 25, 50, 75, 100)
  let ind = 0
  
  align(center, table(
  align: center,
    table.header(h_cell("Problem size"), h_cell("Avg. time for setup"), h_cell("Avg. time for solving")),
    columns: 3,
    inset: 0.6em,
    fill: (_, row) => 
      if calc.odd(row) {
        tab-colors.dispari
      } else {tab-colors.pari},
      ..dataset
    )
  )
}

#let SAParameters_single(data) = {
  align(center, table(
  align: (x, y) => if y == 0 {center + horizon} else {center},
    table.header(h_cell($N_(m a x)$), h_cell($alpha$), h_cell($T$), h_cell($N_(b a d)$), h_cell($P_(2 o p t)$), h_cell($P_(3 o p t)$)),
    columns: 6 * (14%,),
    inset: 0.6em,
    fill: (_, row) => 
      if calc.odd(row) {
        tab-colors.dispari
      } else {tab-colors.pari},
      ..data
    )
  )
}

#let SAParameters(data) = {
  align(center, table(
  align: (x, y) => if y == 0 {center + horizon} else {center},
    table.header(h_cell("Size"), h_cell($N_(m a x)$), h_cell($alpha$), h_cell($T$), h_cell($N_(b a d)$), h_cell($P_(2 o p t)$), h_cell($P_(3 o p t)$)),
    columns: 7 * (12%,),
    inset: 0.6em,
    fill: (_, row) => 
      if calc.odd(row) {
        tab-colors.dispari
      } else {tab-colors.pari},
      ..data
    )
  )
}

#let performances_comp(dataset) = {
  let data = csv(dataset).flatten()
  data.at(0) = raw(data.at(0))
  let value = 7
  //set name file to raw
  while value < (data.len() -1){
    if calc.rem(value, 7) == 0 {
      data.at(value) = raw(data.at(value))
    }
    value = value + 1
  }
  align(center, table(
  align: (x, y) => if y == 0 {center + horizon} else {center},
    table.header(h_cell("Instance"), h_cell("Cplex Sol."), h_cell("Cplex Solving Time"), h_cell("SA Sol."), h_cell("SA Solving Time"), h_cell("Gap %"), h_cell("Speedup SA")),
    columns: 7,
    inset: 0.6em,
    fill: (_, row) => 
      if calc.odd(row) {
        tab-colors.dispari
      } else {tab-colors.pari},
      ..data
    )
  )
}

#let SAPerformances(training) = {
  let data = csv(training).flatten()
  data.at(0) = raw(data.at(0))
  let value = 6
  //set name file to raw
  while value < (data.len() -1){
    if calc.rem(value, 6) == 0 {
      data.at(value) = raw(data.at(value))
    }
    value = value + 1
  }
  let t_rows = 6
  let el_rows = 6
  let training = data.slice(0,t_rows * el_rows )
  let testing = data.slice(t_rows * el_rows, data.len())
  align(center, table(
  align: (x, y) => if y == 0 {center + horizon} else {center},
    columns: 6,
    inset: 0.6em,
    fill: (_, row) => 
      if calc.odd(row) {
        tab-colors.dispari
      } else {tab-colors.pari},
      table.cell(text([*Training*], fill: white), align: center, colspan: 6, fill: tab-colors.head ),
      h_cell("Instance"), h_cell("Best Sol."), h_cell("Worst Sol."), h_cell("Avg."), h_cell("Std. Dev."), h_cell("Avg. Time"),
      ..training,
      table.cell(text([*Testing*], fill: white), align: center, colspan: 6, fill: tab-colors.head),
      h_cell("Instance"), h_cell("Best Sol."), h_cell("Worst Sol."), h_cell("Avg."), h_cell("Std. Dev."), h_cell("Avg. Time"),
      ..testing
    )
  )
}

#let fig(lab, name) = {
  show link: set text(fill: blue)
  show link: underline
  link(label(lab), name)
}

#let code(t, lan: "c++") = {
  h(2pt)
  box(fill: luma(220), raw(t, lang: lan), outset: 2.5pt, stroke: black + 0.3pt,)
  h(2pt)
}