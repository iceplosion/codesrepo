// Reference: https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/template

// test if browser support <template>
if ('content' in document.createElement('template')) {
  // get the template element by id
  let tplt = document.querySelector('#template-row');

  // get list of sub-elements <td> of <template>
  tds = tplt.content.querySelectorAll("td");

  // change the textContent value of td
  tds[0].textContent = 'Alice';
  tds[1].textContent = 'abc@xyz.com'
  // copy element in <template> (which can be get from tplt.content)
  let row1 = document.importNode(tplt.content, true);
  
  tds[0].textContent = 'Bob';
  tds[1].textcontent = 'xyz@abc.com';
  let row2 = document.importNode(tplt.content, true);

  // get <tbody> element
  let tb = document.getElementsByTagName("tbody");
  // insert instance of template into <tbody>
  tb[0].appendChild(row1);
  tb[0].appendChild(row2);
} else {
  // do something at here if browser doesn't support <template>
}