/*global $, jQuery, graphApi graphDraw */

var graphInteract = (function() {
  var defaultPauseTime = 200;
  var interval;
  var graphRenderer = graphDraw.forceRenderer;
  var isPlaying = false;

  function startIteration() {
    var pauseTime = $('#step').val() || defaultPauseTime;
    console.log('starting interval with pause time ' + pauseTime);
    interval = setInterval(function() {
      graphApi.iterate();
      graphRenderer.render();
    }, pauseTime);
    isPlaying = true;
    $('#pause').text('Pause');
  }

  function stopIteration() {
    clearInterval(interval);
    isPlaying = false;
    $('#pause').text('Play');
  }

  function toggleIteration() {
    if (isPlaying) {
      stopIteration();
    } else {
      startIteration();
    }
  }

  function createGraph() {
    var x = $('#x').val();
    var y = $('#y').val();
    graphApi.create(x, y);
    graphDraw.clear();
    graphRenderer.render();
  }

  return {
    toggleIteration: toggleIteration,
    createGraph: createGraph
  };
}());
