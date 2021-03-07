const numSteps = 20.0;

let box1;
let box2;
let color_black = "rgba(0, 0, 0, ratio)";
let color_grey = "rgba(20, 20, 20, ratio)";

// Set things up
window.addEventListener("load", (event) => {
  box1 = document.querySelector("#root");
  box2 = document.querySelector("#child");
  createObserver();
}, false);

function createObserver() {
    let observer;
  
    let options1 = {
      root: null,
      rootMargin: "0px",
      threshold: buildThresholdList()
    };
    observer = new IntersectionObserver(handleIntersect1, options1);
    observer.observe(box1);

    let options2 = {
      root: box1,
      rootMargin: "0px",
      threshold: buildThresholdList()
    };
    observer = new IntersectionObserver(handleIntersect2, options2);
    observer.observe(box2);
}
function buildThresholdList() {
    let thresholds = [];
    let numSteps = 20;
  
    for (let i=1.0; i<=numSteps; i++) {
      let ratio = i/numSteps;
      thresholds.push(ratio);
    }
  
    thresholds.push(0);
    return thresholds;
}

function handleIntersect1(entries, observer) {
    entries.forEach((entry) => {
        entry.target.style.backgroundColor = color_grey.replace("ratio", entry.intersectionRatio);
    });
}

function handleIntersect2(entries, observer) {
    entries.forEach((entry) => {
        entry.target.style.backgroundColor = color_black.replace("ratio", entry.intersectionRatio);
    });
}
