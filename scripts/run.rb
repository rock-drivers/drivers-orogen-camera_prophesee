require 'orocos'
require 'vizkit'
include Orocos

ENV['PKG_CONFIG_PATH'] = "#{File.expand_path("..", File.dirname(__FILE__))}/build:#{ENV['PKG_CONFIG_PATH']}"

Orocos::CORBA::max_message_size = 100000000000
Orocos.initialize

Orocos::Process.run 'camera_prophesee::Task' => 'camera_prophesee',
                    'camera_prophesee::FrameGenerator' => 'frame_generator' do
    # log all the output ports
    Orocos.log_all_ports 
    Orocos.conf.load_dir('../config')

    # Get the task
    driver = Orocos.name_service.get 'camera_prophesee'
    Orocos.conf.apply(driver, ['default','prophesee_yellow_dot'], :override => true)

    # Get the frame generator
    generator = Orocos.name_service.get 'frame_generator'
    Orocos.conf.apply(generator, ['default'], :override => true)

    # Configure
    driver.configure
    generator.configure

    # Connect ports
    driver.events.connect_to generator.events, :type => :buffer, :size => 2

    # Start
    driver.start
    generator.start

    Vizkit.exec
end
