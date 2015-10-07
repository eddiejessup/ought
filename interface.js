/*global $, jQuery, d3, graph */

$(document).ready(function() {
  var defaultPauseTime = 100;
  var interval;

  function startIteration() {
    var pauseTime = $('#step').val() || defaultPauseTime;
    console.log(pauseTime);
    interval = setInterval(function() {
      graph.iterate();
      graph.render();
    }, pauseTime);
  }

  function toggleIteration() {
    if (isPlaying) {
      clearInterval(interval);
      isPlaying = false;
      $('#pause').text('Play');
    } else {
      startIteration();
      isPlaying = true;
      $('#pause').text('Pause');
    }
  }

  function createGraph() {
    var x = $('#x').val();
    var y = $('#y').val();
    graph.clear();
    graph.create(x, y);
    graph.render();
  }

  $('#pause').on('click', toggleIteration);
  $('#reset').on('click', createGraph);
  $('#create').on('click', createGraph);

  var isPlaying = false;
});
