require 'orocos'
require 'vizkit'
include Orocos

ENV['PKG_CONFIG_PATH'] = "#{File.expand_path("..", File.dirname(__FILE__))}/build:#{ENV['PKG_CONFIG_PATH']}"

Orocos.initialize

Orocos::Process.run 'camera_prophesee::Task' => 'camera_prophesee' do
    # log all the output ports
    Orocos.log_all_ports 
    Orocos.conf.load_dir('.')

    # Get the task
    driver = Orocos.name_service.get 'camera_prophesee'
    Orocos.conf.apply(driver, ['default','prophesee_yellow_dot'], :override => true)

    driver.configure
    driver.start

    Vizkit.exec
end
