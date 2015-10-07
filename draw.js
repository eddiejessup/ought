/*global $, jQuery, d3*/

var graph = (function () {
  var domain = "http://localhost:5000";
  var color = d3.scale.category10();

  function _getGraphData(callback) {
    $.ajax({
      context: this.graph,
      method: "GET",
      url: domain,
      dataType: "json",
      crossDomain: true
    }).done(callback);
  }

  function render() {
    var renderInitial = function (graphData) {
      var svg = d3.select("svg");
      var width = svg.attr("width");
      var height = svg.attr("height");

      var force = d3.layout.force()
        .charge(-120)
        .linkDistance(10)
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
        .attr("r", 5)
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
    _getGraphData(renderFunc);
  }

  function clear() {
    d3.select("svg").selectAll("*").remove();
  }

  function iterate() {
    $.ajax({
      method: "GET",
      url: domain + '/iterate',
      dataType: "text",
      crossDomain: true,
    }).done(function (data) { console.log(data); });
  }

  function create(x, y) {
    $.ajax({
      method: "GET",
      url: domain + '/create/' + x + ',' + y,
      dataType: "text",
      crossDomain: true
    }).done(function (data) { console.log(data); });
  }

  return {
    create: create,
    render: render,
    clear: clear,
    iterate: iterate,
  };
}());
