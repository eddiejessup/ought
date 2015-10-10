/*global $, jQuery, d3, graphApi*/

var graphDrawForce = (function () {
  var color = d3.scale.category10();
  var circleRadius = 5;
  var charge = -180;
  var linkDistance = 10;

  function render() {
    var renderInitial = function (graphData) {
      var svg = d3.select("svg");
      var width = svg.attr("width");
      var height = svg.attr("height");

      var force = d3.layout.force()
        .charge(charge)
        .linkDistance(linkDistance)
        .size([width, height]);

      force.nodes(graphData.nodes).links(graphData.links).start();

      var link = svg.selectAll(".link")
        .data(graphData.links)
        .enter()
        .append("line")
        .attr("class", "link");

      var node = svg.selectAll(".node")
        .data(graphData.nodes)
        .enter()
        .append("circle")
        .attr("class", "node")
        .attr("r", circleRadius)
        .style("fill", function (d) {
          return color(d.state);
        })
        .call(force.drag);

      node.append("title").text(function (d) { return d.name; });

      force.on("tick", function () {
        link.attr("x1", function (d) { return d.source.x; })
          .attr("y1", function (d) { return d.source.y; })
          .attr("x2", function (d) { return d.target.x; })
          .attr("y2", function (d) { return d.target.y; });

        node.attr("cx", function (d) { return d.x; })
          .attr("cy", function (d) { return d.y; });
      });

      this.links = svg.selectAll(".link");
      this.nodes = svg.selectAll(".node");

    };

    var renderExisting = function (graphData) {
      this.nodes.style("fill", function (_, i) {
        return color(graphData.nodes[i].state);
      });
    };

    var renderFunc;
    if (d3.select("svg").selectAll('.link')[0].length) {
      renderFunc = renderExisting;
    } else {
      renderFunc = renderInitial;
    }
    graphApi.getGraph(this, renderFunc);
  }

  function clear() {
    d3.select("svg").selectAll("*").remove();
  }

  return {
    render: render,
    clear: clear,
  };
}());
