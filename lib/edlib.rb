require_relative 'edlib/edlib'

module Edlib
  class Aligner
    def initialize(k:-1, mode: 'NW', task: 'DISTANCE', additional_equalities: nil)
      mode = mode.to_s if mode.is_a? Symbol
      task = task.to_s if task.is_a? Symbol
      mode = mode.upcase if mode.is_a? String
      task = task.upcase if task.is_a? String
      initialize_raw(k, mode, task, additional_equalities)
    end
  end
end