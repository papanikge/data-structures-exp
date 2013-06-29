#!/usr/bin/env ruby

# Warning: This is not flexible and not intended to be used widely
# even the patterns and the files are hardcoded

require 'nokogiri'

doc = Nokogiri::XML(open("new.xml"))

# we will feed some random data
# since we don't really care about truthness
def get_random_span(from, to)
  rand(to - from) + from
end

titles = doc.css('bht').map do |item|
  # keeping only what we need
  item = item['title']
end

authors = doc.xpath('//bht/ul/li/author').map do |item|
  # returns an array, so we iterate to strip spaces and xml tags
  item = item.content.lstrip
end

editors = doc.xpath('//bht/p/editor').map do |item|
  item = item.content.lstrip
end

# start constructing the file
File.open("datafile.new", 'w') do |f|
  1000000.times do
    t = titles.sample
    next if t == "..." or t =~ /\d/
    # randomly change two bytes, because why not..
    t[get_random_span(0, t.size - 1)] = 'q'
    t[get_random_span(0, t.size - 1)] = 'b'
    # here is the pattern
    f.write("\"#{get_random_span(0,999999999)}\";\"#{t}\";\"#{authors.sample}\";\"#{get_random_span(1970, 2013)}\";\"#{editors.sample} Inc.\"\n")
  end
end
