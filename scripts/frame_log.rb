require 'orocos'
require 'orocos/log'
require 'rock/bundle'
require 'vizkit'
require 'utilrb'


include Orocos

Bundles.initialize

Orocos::Process.run 'camera_prophesee::FrameGenerator' => 'frame_prophesee' do

   ## Get the task context ##
  STDERR.print "setting up frame_prophesee..."
  frame_prophesee = TaskContext.get 'frame_prophesee'
  Orocos.conf.apply(frame_prophesee, ['default'], :override => true)
  frame_prophesee.configure


  # logs files
  log_replay = Orocos::Log::Replay.open( ARGV[0] )
  log_replay.camera_prophesee.events.connect_to(frame_prophesee.events, :type => :buffer, :size => 10)

  # start the task
  frame_prophesee.start

  # open the log replay widget
  control = Vizkit.control log_replay
  control.speed = 1

  Vizkit.exec

end

