unsigned int
shift_left_with_wrap(unsigned int bits, unsigned int amount)
{
  return (bits << amount | bits >> (NUMTAGS - amount)) & TAGMASK;
}

unsigned int
shift_right_with_wrap(unsigned int bits, unsigned int amount)
{
  return (bits >> amount | bits << (NUMTAGS - amount)) & TAGMASK;
}

unsigned int
shift_left_along_mask(unsigned int bits, unsigned int mask)
{
  unsigned int shifted_bits = 0;

  while (mask != 0 && bits != 0) {
    unsigned int current_bits = bits;
    unsigned int found_bits   = 0;
    unsigned int i            = 0;

    do {
      ++i;
      current_bits = shift_left_with_wrap(current_bits, 1);
    } while (!(found_bits = current_bits & mask));

    bits         ^= shift_right_with_wrap(found_bits, i);
    mask         ^= found_bits;
    shifted_bits |= found_bits;
  }

  return shifted_bits;
}

unsigned int
shift_right_along_mask(unsigned int bits, unsigned int mask)
{
  unsigned int shifted_bits = 0;

  while (mask != 0 && bits != 0) {
    unsigned int current_bits = bits;
    unsigned int found_bits   = 0;
    unsigned int i            = 0;

    do {
      ++i;
      current_bits = shift_right_with_wrap(current_bits, 1);
    } while (!(found_bits = current_bits & mask));

    bits         ^= shift_left_with_wrap(found_bits, i);
    mask         ^= found_bits;
    shifted_bits |= found_bits;
  }

  return shifted_bits;
}

void
nextview(const Arg* arg)
{
  unsigned int current_tags = selmon->tagset[selmon->seltags];

  Client*      c;
  unsigned int occ = 0;
  for (c = selmon->clients; c; c = c->next) {
    occ |= c->tags == 255 ? 0 : c->tags;
  }

  if (occ == 0) return;

  unsigned int next_tags          = shift_left_along_mask(current_tags, occ);
  selmon->tagset[selmon->seltags] = next_tags;
  focus(NULL);
  arrange(selmon);
}

void
previousview(const Arg* arg)
{
  unsigned int current_tags = selmon->tagset[selmon->seltags];

  Client*      c;
  unsigned int occ = 0;
  for (c = selmon->clients; c; c = c->next) {
    occ |= c->tags == 255 ? 0 : c->tags;
  }

  if (occ == 0) return;

  unsigned int next_tags          = shift_right_along_mask(current_tags, occ);
  selmon->tagset[selmon->seltags] = next_tags;
  focus(NULL);
  arrange(selmon);
}
