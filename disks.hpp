///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the 
// alternating disks problem.
//
// As provided, this header has four functions marked with 
// TODO comments.
// You need to write in your own implementation of these 
// functions.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(dark_count() > 0);
      //assert(light_count > 0);

      //for (size_t i = 0; i < _colors.size(); i += 2) {
      for (size_t i = 1; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t dark_count() const {
    return light_count();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
    int even = 0;

    // from first index 
    for (int idx = 0; idx < _colors.size(); idx++){

      // starting from
      // check even indexes if DISK_DARK
      if (idx % 2 == even){
        if (_colors[idx] == DISK_DARK){
          return false;
        }
      } 
      else {
        // check even indexes if DISK_LIGHT
        if (_colors[idx] == DISK_LIGHT){
          return false;
        }
      }
    }
    return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
    int swaps = 0; // swap counter

    // check if first index is DISK_DARK
    // if DISK_DARK, not sorted
    if (_colors[0] == DISK_DARK){
      return false;
    }
    else {

      // starting from second index until last, compare
      // adjacent indexes and return false if no swaps occur 
      for (int idx = 1; idx < _colors.size() - 1; idx++){
        if (_colors[idx] != _colors[idx + 1]){
          swaps++;
          if (swaps > 1){
            return false;
          }
        }
      }
    }
   return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
  disk_state curr_state = before;
  int swaps = 0; // swap counter
  int curr_pos = 0; // current position
  int nxt = 1; // position after current

  while (!curr_state.is_sorted()){
    if (curr_state.get(curr_pos) == DISK_DARK && curr_state.get(nxt) == DISK_LIGHT){
      curr_state.swap(curr_pos);
      swaps++;
    }

    // increment both positions
    curr_pos++;
    nxt++;

    // check if positions are last two positions in vector
    if (curr_pos == curr_state.total_count() - 2 || nxt == curr_state.total_count() - 1){
      curr_pos = 0;
      nxt = 1;
    }
  }

  // check that the input is in alternating format  
  assert(before.is_alternating());
  
  return sorted_disks(curr_state, swaps);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  disk_state curr_state = before;
  int swaps = 0; // swap counter

  while (!curr_state.is_sorted()){

    // checking left to right for swaps
    for (int idx = 0; idx < curr_state.total_count()-1; idx++){
      if (curr_state.get(idx) == DISK_DARK && curr_state.get(idx+1) == DISK_LIGHT){
        curr_state.swap(idx);
        swaps++;
      }
    }

    // checking right to left for swaps
    for (int idx = curr_state.total_count()-1; idx > 0; idx--){
      if (curr_state.get(idx) == DISK_LIGHT && curr_state.get(idx-1) == DISK_DARK){
        curr_state.swap(idx-1);
        swaps++;
      }
    }
  }
  
  assert(before.is_alternating());

  return sorted_disks(curr_state, swaps);
}
