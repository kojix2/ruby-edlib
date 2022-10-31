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
  end
end
