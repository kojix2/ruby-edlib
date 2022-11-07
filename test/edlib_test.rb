require_relative 'test_helper'

class EdlibTest < Test::Unit::TestCase
  def test_that_it_has_a_version_number
    refute_nil ::Edlib::VERSION
  end

  def test_initilize
    assert_instance_of Edlib::Aligner, Edlib::Aligner.new
  end

  def test_config
    assert_instance_of Hash, Edlib::Aligner.new.config
  end

  def test_align
    assert_instance_of Hash, Edlib::Aligner.new.align('A', 'A')
  end

  def test_k
    a = Edlib::Aligner.new
    assert_equal(-1, a.k)
    a.k = 2
    assert_equal 2, a.k
    assert_raise { a.k = [] }
  end

  def test_mode
    a = Edlib::Aligner.new
    assert_equal('NW', a.mode)
    a.mode = 1
    assert_equal('SHW', a.mode)
    a.mode = 'hw'
    assert_equal('HW', a.mode)
    a.mode = :nw
    assert_equal('NW', a.mode)
    assert_raise { a.mode = 3 }
    assert_raise { a.mode = -1 }
    assert_raise { a.mode = 'NN' }
    assert_raise { a.mode = [] }
  end

  def test_task
    a = Edlib::Aligner.new
    assert_equal('DISTANCE', a.task)
    a.task = 1
    assert_equal('LOC', a.task)
    a.task = 'path'
    assert_equal('PATH', a.task)
    a.task = :distance
    assert_equal('DISTANCE', a.task)
    assert_raise { a.task = 3 }
    assert_raise { a.task = -1 }
    assert_raise { a.task = 'KOJIX' }
    assert_raise { a.task = [] }
  end

  def test_additional_equalities
    a = Edlib::Aligner.new
    assert_equal [], a.additional_equalities
    a.additional_equalities = [%w[A B], %w[C D]]
    assert_equal [%w[A B], %w[C D]], a.additional_equalities
    a.additional_equalities = []
    assert_equal [], a.additional_equalities
  end

  def test_align
    a = Edlib::Aligner.new
    r = a.align('telephone', 'elephant')
    assert_equal 3, r[:edit_distance]
  end

  def test_align_additional_equalities
    a = Edlib::Aligner.new(mode: :hw, additional_equalities: [%w[R A], %w[R G]])
    r = a.align('ACTG', 'CACTRT')
    assert_equal 0, r[:edit_distance]
  end

  def test_align_nice_nw
    a = Edlib::Aligner.new(mode: :nw, task: :path)
    r = a.align('TAAGGATGGTCCCATTC', 'AAGGGGTCTCATATC', nice: true)
    assert_equal 'TAAGGATGGTCCCAT-TC', r[:query_aligned]
    assert_equal '-||||--||||.|||-||', r[:match_aligned]
    assert_equal '-AAGG--GGTCTCATATC', r[:target_aligned]
  end

  def test_align_nice_hw
    a = Edlib::Aligner.new(mode: :hw, task: :path)
    r = a.align('TAAGGATGGTCCCATTC', 'AAGGGGTCTCATATC', nice: true)
    assert_equal 'TAAGGATGGTCCCAT-TC', r[:query_aligned]
    assert_equal '-||||--||||.|||-||', r[:match_aligned]
    assert_equal '-AAGG--GGTCTCATATC', r[:target_aligned]
  end

  def test_align_nice_shw
    a = Edlib::Aligner.new(mode: :shw, task: :path)
    r = a.align('TAAGGATGGTCCCATTC', 'AAGGGGTCTCATATC', nice: true)
    assert_equal 'TAAGGATGGTCCCAT-TC', r[:query_aligned]
    assert_equal '-||||--||||.|||-||', r[:match_aligned]
    assert_equal '-AAGG--GGTCTCATATC', r[:target_aligned]
  end
end
