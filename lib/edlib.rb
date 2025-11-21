# frozen_string_literal: true

require_relative 'edlib/edlibext'

module Edlib
  class Aligner
    def initialize(k: -1, mode: 'NW', task: 'DISTANCE', additional_equalities: nil)
      mode = mode.to_s if mode.is_a? Symbol
      task = task.to_s if task.is_a? Symbol
      mode = mode.upcase if mode.is_a? String
      task = task.upcase if task.is_a? String
      initialize_raw(k, mode, task, additional_equalities)
    end

    def align(query, target, nice: false)
      if nice
        align_nice(query, target)
      else
        align_raw(query, target)
      end
    end

    private

    def align_nice(query, target)
      result = align_raw(query, target)
      result.merge(nice(result, query, target))
    end

    def nice(result, query, target, gap_symbol: '-')
      raise 'result does not have :locations and :cigar' unless result.key?(:locations) && result.key?(:cigar)

      target_pos = result[:locations][0][0]
      query_pos = 0
      query_aln = String.new
      match_aln = String.new
      target_aln = String.new
      cigar = result[:cigar]
      cigar.scan(/(\d+)(\D)/).each do |num, op|
        num = num.to_i
        case op
        when '='
          target_aln << target[target_pos, num]
          target_pos += num
          query_aln << query[query_pos, num]
          query_pos += num
          match_aln << '|' * num
        when 'X'
          target_aln << target[target_pos, num]
          target_pos += num
          query_aln << query[query_pos, num]
          query_pos += num
          match_aln << '.' * num
        when 'D'
          target_aln << target[target_pos, num]
          target_pos += num
          query_aln << gap_symbol * num
          query_pos += 0
          match_aln << gap_symbol * num
        when 'I'
          target_aln << gap_symbol * num
          target_pos += 0
          query_aln << query[query_pos, num]
          query_pos += num
          match_aln << gap_symbol * num
        else
          raise "Unknown CIGAR operation: #{op}"
        end
      end

      {
        query_aligned: query_aln,
        match_aligned: match_aln,
        target_aligned: target_aln
      }
    end
  end
end
